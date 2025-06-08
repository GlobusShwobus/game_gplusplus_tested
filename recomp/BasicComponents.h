#pragma once

#include "SDL3/SDL.h"
#include <map>
#include <vector>
#include <random>
#include <array>

//cool new stuff

typedef uint32_t HASH_ID_TYPE;

constexpr HASH_ID_TYPE HASH(const char* str) {
	uint32_t hashish = 2166136261u;
	while (*str) {
		hashish ^= *str++;
		hashish *= 16777619u;
	}
	return hashish;
}

//####################################################################################################
//--------------------   ANIMATION IDs   -------------------------------------------------------------
//####################################################################################################
typedef HASH_ID_TYPE AnimID;
constexpr AnimID AnimID_WALK_UP = HASH("walk_up");
constexpr AnimID AnimID_WALK_DOWN = HASH("walk_down");
constexpr AnimID AnimID_WALK_LEFT = HASH("walk_left");
constexpr AnimID AnimID_WALK_RIGHT = HASH("walk_right");
constexpr AnimID AnimID_IDLE_UP = HASH("idle_up");
constexpr AnimID AnimID_IDLE_DOWN = HASH("idle_down");
constexpr AnimID AnimID_IDLE_LEFT = HASH("idle_left");
constexpr AnimID AnimID_IDLE_RIGHT = HASH("idle_right");

//####################################################################################################
//--------------------   ENTITY TYPES   --------------------------------------------------------------
//####################################################################################################
typedef HASH_ID_TYPE EntityType;
typedef HASH_ID_TYPE EntityGeneric;
typedef EntityGeneric EnemyID;
typedef EntityGeneric PlayerID;

constexpr EntityType EntityType_PLAYER = HASH("player_type");
constexpr EntityType EntityType_ENEMY = HASH("enemy_type");

constexpr bool definedEntityType(const EntityType hashedType) {
	return hashedType == EntityType_PLAYER || hashedType == EntityType_ENEMY;
}

struct FrameMap {
	AnimID id = 0;//int
	std::vector<SDL_Rect> frames;
	bool isLooping = false;
};
class AnimController {
	static constexpr int frameDelay = 6;

	const std::vector<FrameMap>& clips;
	const FrameMap* current = nullptr;

	int frameIndex = 0;
	int frameTimer = 0;
public:
	AnimController(const std::vector<FrameMap>& reels);
	void moveFrame();
	bool setIfNew(const AnimID id);
	void applySourceFromFrame(SDL_FRect& rect)const;
};

enum MyPersonalEvents : Uint64 {
	MPE_cleanup = 1 << 0,
	MPE_movingObject = 1 << 1,
	MPE_checkAnimation = 1 << 2,
	MPE_checkDirection = 1 << 3,
};

struct EntityEvent {
	Uint64 events = 0;
	bool containsEvent(MyPersonalEvents event)const;
	static bool containsEvent(const Uint64& events, MyPersonalEvents event);
	void setEvent(const int events);
	void flushEvents();
};

enum class Direction {
	right, down, left, up, none
};

struct Transform {
	SDL_FRect rect{ 0,0,0,0 };
	SDL_FPoint velocity{ 0,0 };

	float mass = 0;

	Transform() = default;
	Transform(float x, float y, float w, float h) :rect{ x,y,w,h } {}

	void updatePos() {
		rect.x = rect.x + velocity.x;
		rect.y = rect.y + velocity.y;
	}
	static bool containsPoint(const SDL_FRect& container, const SDL_FPoint& point) {
		return (point.x >= container.x && point.y >= container.y && point.x < container.x + container.w && point.y < container.y + container.h);
	}
	bool containsPoint(const SDL_FPoint& point)const {
		return (point.x >= rect.x && point.y >= rect.y && point.x < rect.x + rect.w && point.y < rect.y + rect.h);
	}
	bool containsRect(const SDL_FRect& inner)const {
		return (inner.x >= rect.x && inner.y >= rect.y && inner.x + inner.w <= rect.x + rect.w && inner.y + inner.h <= rect.y + rect.h);
	}
	bool basicAABBcollision(const SDL_FRect& another)const {
		return (rect.x < another.x + another.w && rect.x + rect.w > another.x && rect.y < another.y + another.h && rect.y + rect.h > another.y);
	}
	//used to predict a collision, not checking if currently collides
	bool containsLine(const SDL_FRect& target){
		// Cache division
		SDL_FPoint inverse = { 1.0f / velocity.x, 1.0f / velocity.y };
		// Calculate intersections with rectangle bounding axes
		SDL_FPoint tNear = {
			(target.x - rect.x) * inverse.x,
			(target.x - rect.y) * inverse.y
		};
		SDL_FPoint tFar = {
			(target.x + target.w - rect.x) * inverse.x,
			(target.y + target.h - rect.y) * inverse.y
		};

		if (std::isnan(tFar.y) || std::isnan(tFar.x)) return false;
		if (std::isnan(tNear.y) || std::isnan(tNear.x)) return false;

		// Sort distances
		if (tNear.x > tFar.x) std::swap(tNear.x, tFar.x);
		if (tNear.y > tFar.y) std::swap(tNear.y, tFar.y);

		// Early rejection, no intersection		
		if (tNear.x > tFar.y || tNear.y > tFar.x) return false;

		// Closest 'time' will be the first contact
		float tHitNear = std::max(tNear.x, tNear.y);

		// Furthest 'time' is contact on opposite side of target
		float tHitFar = std::min(tFar.x, tFar.y);

		// Reject if ray direction is pointing away from object
		if (tHitFar < 0) // May have to check tHitNear as well because i am a dumbo
			return false;
		return tHitNear <= 1.0f;
	}
	bool enhancedAABB(const SDL_FRect& target) {

		// Calculate half sizes for center-based collision
		float aHalfW = rect.w * 0.5f;
		float aHalfH = rect.h * 0.5f;
		float bHalfW = target.w * 0.5f;
		float bHalfH = target.h * 0.5f;

		// Calculate centers
		SDL_FPoint aCenter = {
			rect.x + aHalfW,
			rect.y + aHalfH
		};
		SDL_FPoint bCenter = {
			target.x + bHalfW,
			target.y + bHalfH
		};

		// Calculate distance between centers
		float dx = bCenter.x - aCenter.x;
		float dy = bCenter.y - aCenter.y;
		float minDistX = aHalfW + bHalfW;
		float minDistY = aHalfH + bHalfH;

		// Early exit if no overlap possible
		if (abs(dx) > minDistX || abs(dy) > minDistY) {
			return false; // isColliding = false
		}

		// Calculate overlap amounts
		float overlapX = minDistX - abs(dx);
		float overlapY = minDistY - abs(dy);



		return true;
	}
	void flipNormalized(SDL_FPoint& normalized) {
		normalized.x = -normalized.x;
		normalized.y = -normalized.y;
	}
	void reflectVelocity(const SDL_FPoint& normalized) {
		
		if (normalized.x != 0.0f) {
			velocity.x = -velocity.x;
		}
		if (normalized.y != 0.0f) {
			velocity.y = -velocity.y;
		}

	}
	SDL_FPoint getNormalizedSign(const SDL_FPoint& velocity) {
		SDL_FPoint out = { 0.0f, 0.0f };
		if (velocity.x > 0.0f) 
			out.x = 1.0f;
		else if (velocity.x < 0.0f) 
			out.x = -1.0f;
		if (velocity.y > 0.0f) 
			out.y = 1.0f;
		else if (velocity.y < 0.0f) 
			out.y = -1.0f;
		return out;
	}
	void clearVelocity() {
		velocity = { 0,0 };
	}
	void clampInOf(const SDL_FRect& outer) {
		if (rect.x < outer.x) {
			rect.x = outer.x;
		}
		if (rect.y < outer.y) {
			rect.y = outer.y;
		}
		if (rect.x + rect.w > outer.w) {
			rect.x = outer.w - rect.w;
		}
		if (rect.y + rect.h > outer.h) {
			rect.y = outer.h - rect.h;
		}
	}
	static void clampInOf(const SDL_FRect& outer, SDL_FRect& inner) {
		if (inner.x < outer.x) {
			inner.x = outer.x;
		}
		if (inner.y < outer.y) {
			inner.y = outer.y;
		}
		if (inner.x + inner.w > outer.w) {
			inner.x = outer.w - inner.w;
		}
		if (inner.y + inner.h > outer.h) {
			inner.y = outer.h - inner.h;
		}
	}

	void velReflectWithMass(SDL_FPoint& othersVel, float othersMass, const SDL_FPoint& normalized) {
		SDL_FPoint relativeVel = {velocity.x - othersVel.x,velocity.y - othersVel.y};

		float velAlongNormal = relativeVel.x * normalized.x + relativeVel.y * normalized.y;
		if (velAlongNormal > 0.0f) return;

		const float restitution = 1.0f;

		float j = -(1.0f + restitution) * velAlongNormal;
		j /= (1.0f / mass) + (1.0f / othersMass);

		SDL_FPoint impulse = { j * normalized.x,j * normalized.y };

		velocity.x += impulse.x / mass;
		velocity.y += impulse.y / mass;

		othersVel.x -= impulse.x / othersMass;
		othersVel.y -= impulse.y / othersMass;
	}
};
class RandomNumberGenerator {
	std::random_device rd;
	std::unique_ptr<std::mt19937> rng;

public:
	RandomNumberGenerator(): rng(std::make_unique<std::mt19937>(rd())) {}

	int getRand(int min, int max) {
		return std::uniform_int_distribution<int>(min, max)(*rng);
	}
};

struct EntityData {
	EntityGeneric id = 0;
	Transform transform;
	float movement_speed = 0.f;
	float health_points = 0.f;
	float mass = 0.f;
	float frameWidth = 0.f;
	float frameHeight = 0.f;
};

class Player {

public:
	SDL_Texture* texture = nullptr;//not owner
	SDL_FRect textureSrc{ 0,0,0,0 };
	SDL_FRect textureDest{ 0,0,0,0 };
	AnimController animControlls;

	Transform transform;

	EntityEvent events;
	Direction direction = Direction::none;


	Player(SDL_Texture* texture, const std::vector<FrameMap>& frameMap, const EntityData& data):texture(texture), animControlls(frameMap) {
		textureSrc = { 0,0,data.frameWidth, data.frameHeight };
		textureDest = { 0,0,data.frameWidth, data.frameHeight };

		transform = data.transform;
	}

	void applyCollisionBoxToRenderBox() {
		textureDest = transform.rect;
	}
	void applySourceBoxToRenderBox() {
		animControlls.applySourceFromFrame(textureSrc);
	}
};

class EnemyBasic {
public:
	SDL_Texture* texture = nullptr;//not owner
	SDL_FRect textureSrc{ 0,0,0,0 };
	SDL_FRect textureDest{ 0,0,0,0 };

	Transform transform;

	EnemyBasic(SDL_Texture* texture, const EntityData& data):texture(texture) {
		textureSrc = { 0,0,data.frameWidth, data.frameHeight };
		textureDest = { 0,0,data.frameWidth, data.frameHeight };

		transform = data.transform;
	}
};
