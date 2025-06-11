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

	float halfWidth = 0;
	float halfHeight = 0;

	Transform() = default;
	Transform(float x, float y, float w, float h) :rect{ x,y,w,h } {}
	Transform(const SDL_FRect& rectangle, const SDL_FPoint& vel) :rect(rectangle), velocity(vel), halfWidth(rectangle.w*0.5f), halfHeight(rectangle.h*0.5f) {}

	void noResolutionMove() {
		rect.x += velocity.x;
		rect.y += velocity.y;
	}
	bool containsPoint(const SDL_FPoint& point)const {
		return (point.x >= rect.x && point.y >= rect.y && point.x < rect.x + rect.w && point.y < rect.y + rect.h);
	}
	bool containsRect(const SDL_FRect& inner)const {
		return (inner.x >= rect.x && inner.y >= rect.y && inner.x + inner.w <= rect.x + rect.w && inner.y + inner.h <= rect.y + rect.h);
	}
	bool basicAABBcollision(const SDL_FRect& origin, const SDL_FRect& target)const {
		return (
			origin.x < target.x + target.w &&
			origin.x + origin.w > target.x &&
			origin.y < target.y + target.h &&
			origin.y + origin.h > target.y
			);
	}
	bool staticSweptAABB(const SDL_FPoint& rayOrigin, const SDL_FPoint& vector, const SDL_FRect& expandedTarget, SDL_FPoint& contactP, SDL_FPoint& contactN, float& hitTimeEntry) {
		// Inverse direction
	//SDL_FPoint inverse = {
	//	vector.x != 0.0f ? 1.0f / vector.x : INFINITY,
	//	vector.y != 0.0f ? 1.0f / vector.y : INFINITY
	//};
		SDL_FPoint inverse = {
	 1.0f / vector.x,
	 1.0f / vector.y
		};

		// Calculate intersections with rectangle bounding axes
		SDL_FPoint tNear = {
			(expandedTarget.x - rayOrigin.x) * inverse.x,
			(expandedTarget.y - rayOrigin.y) * inverse.y
		};
		SDL_FPoint tFar = {
			(expandedTarget.x + expandedTarget.w - rayOrigin.x) * inverse.x,
			(expandedTarget.y + expandedTarget.h - rayOrigin.y) * inverse.y
		};

		if (SDL_isnan(tNear.x) || SDL_isnan(tNear.y) ||
			SDL_isnan(tFar.x) || SDL_isnan(tFar.y)) return false;

		// Swap near and far
		if (tNear.x > tFar.x) std::swap(tNear.x, tFar.x);
		if (tNear.y > tFar.y) std::swap(tNear.y, tFar.y);

		// Check for ray miss
		if (tNear.x > tFar.y || tNear.y > tFar.x) return false;

		// Time of hit
		hitTimeEntry = std::max(tNear.x, tNear.y);
		float tHitFar = std::min(tFar.x, tFar.y);

		if (tHitFar < 0.0f) return false;


		//contact point
		contactP = {
		(vector.x * hitTimeEntry) + rayOrigin.x,
		(vector.y * hitTimeEntry) + rayOrigin.y
		};

		//normalized
		if (tNear.x > tNear.y)
			if (vector.x < 0)
				contactN = { 1, 0 };
			else
				contactN = { -1, 0 };
		else if (tNear.x < tNear.y)
			if (vector.y < 0)
				contactN = { 0, 1 };
			else
				contactN = { 0, -1 };

		return true;
	}
	bool dynamicSweptAABB(const Transform& target, SDL_FPoint& contactP, SDL_FPoint& contactN,float& hitTimeEntry) {
		// Relative motion
		SDL_FPoint relVelocity = {
			velocity.x - target.velocity.x,
			velocity.y - target.velocity.y
		};

		// Early out if no motion
		if (relVelocity.x == 0.0f && relVelocity.y == 0.0f)
			return false;

		// Expand target rect by this rect's size (swept volume)
		SDL_FRect expandedTarget = {
			target.rect.x - halfWidth,
			target.rect.y - halfHeight,
			target.rect.w + rect.w,
			target.rect.h + rect.h
		};

		// Ray origin is the center of this object
		SDL_FPoint rayOrigin = {
			rect.x + halfWidth,
			rect.y + halfHeight
		};
		if (staticSweptAABB(rayOrigin, relVelocity, expandedTarget, contactP, contactN, hitTimeEntry)) {
			return (hitTimeEntry >= 0.0f && hitTimeEntry < 1.0f);
		}
		return false;
	}
	bool staticAABBOverlap(const Transform& otherRect) {
		float dx = (rect.x + halfWidth) - (otherRect.rect.x + otherRect.halfWidth);
		float dy = (rect.y + halfHeight) - (otherRect.rect.y + otherRect.halfHeight);

		float overlapX = (halfWidth + otherRect.halfWidth) - SDL_fabs(dx);
		float overlapY = (halfHeight + otherRect.halfHeight) - SDL_fabs(dy);

		if (overlapX > 0 && overlapY > 0) {
			// Resolve using smaller overlap
			if (overlapX < overlapY) {
				rect.x += (dx > 0 ? overlapX : -overlapX);
			}
			else {
				rect.y += (dy > 0 ? overlapY : -overlapY);
			}
			return true;
		}
		return false;
	}
	void reflectVelocity(const SDL_FPoint& contactNormal) {
		if (contactNormal.x != 0.0f) {
			velocity.x *= -1;
		}
		if (contactNormal.y != 0.0f) {
			velocity.y *= -1;
		}
	}
	static void flipNormalized(SDL_FPoint& normalized) {
		normalized.x *= -1;
		normalized.y *= -1;
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
