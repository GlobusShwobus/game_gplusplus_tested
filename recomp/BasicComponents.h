#pragma once

#include "SDL3/SDL.h"
#include <map>
#include <vector>

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

struct AnimationReel {
	int beginX = 0;
	int beginY = 0;
	int frameCount = 0;
	bool isLooping = false;
};

class AnimationController {
	static constexpr int frameDelay = 6;
	
	const std::map<AnimID, AnimationReel>* clips = nullptr;//not owner, points to real data
	AnimID currentID = 0;
	const AnimationReel* currentReel = nullptr;//points to clips, which is also not owner
	
	int frameIndex = 0;
	int frameTimer = 0;

public:
	//reels originates from EntityFactory witch holds the data, it is always valid, should not be freed, AND DO NOT FUCK WITH FACTORY MEMORY, otherwise GGWP
	AnimationController(const std::map<AnimID, AnimationReel>& reels) :clips(&reels) {
		if (!reels.empty()) {
			currentID = reels.begin()->first;
			currentReel = &reels.begin()->second;
		}
	}

	void moveFrame();
	void setNewReel(AnimID id);
	//applies source coordinates for the texture
	void applySourceFromFrame(SDL_FRect& rect)const;
};

enum class EntityAction {
	idle, moving, jumping, attacking, dashing
};
enum class EntityDirection {
	left, right, up, down
};
enum EntityEvents {
	directionChange = 1 << 0,
	collsion_immovable = 1 << 1,
	collision_light = 1 << 2,
	collision_medium = 1 << 3,
	collision_hard = 1 << 4,
};
class EntityState {
	bool isActive = false;
	int eventFlags = 0;
	EntityAction action;
	EntityDirection direction;
public:
	EntityState() :action(EntityAction::idle), direction(EntityDirection::down), isActive(true) {}
	void deactivate();
	bool isActivated()const;
	void changeAction(EntityAction action);
	void changeDirection(EntityDirection direction);
	void setEvent(const EntityEvents events);
	void flushEvents();
	int getEvents()const;
	bool containsEvent(EntityEvents event)const;
	EntityDirection getDirection()const;
	EntityAction getAction()const;
};

struct RectTransform {

	SDL_FRect rect{ 0,0,0,0 };
	SDL_FPoint velocity{ 0,0 };
	float halfWidth = 0;
	float halfHeight = 0;

	RectTransform() = default;
	RectTransform(float x, float y, float w, float h) :rect{ x,y,w,h }, halfWidth(w * 0.5f), halfHeight(h * 0.5f) {}

	void setVelocity(const SDL_FPoint& someVel) {
		velocity = someVel;
	}
	void moveOnVector() {
		rect.x += velocity.x;
		rect.y += velocity.y;
	}
};
struct CollisionResult {
	bool isColliding = false;
	SDL_FPoint normal{ 0,0 };
	SDL_FRect overlap{ 0,0,0,0 };
};

struct Collision {
	static bool containsPoint(const SDL_FRect& rect, const SDL_FPoint& point) {
		return (point.x >= rect.x && point.y >= rect.y && point.x < rect.x + rect.w && point.y < rect.y + rect.h);
	}
	static bool containsRect(const SDL_FRect& outer, const SDL_FRect& inner) {
		return (inner.x >= outer.x && inner.y >= outer.y && inner.x + inner.w <= outer.x + outer.w && inner.y + inner.h <= outer.y + outer.h);
	}
	static bool basicAABBcollision(const SDL_FRect& a, const SDL_FRect& b) {
		return (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y);
	}
	static CollisionResult complexAABBcollision(const RectTransform& a, const RectTransform& b) {
		CollisionResult result;

		SDL_FPoint aCenter = { a.rect.x + a.halfWidth, a.rect.y + a.halfHeight };
		SDL_FPoint bCenter = { b.rect.x + b.halfWidth, b.rect.y + b.halfHeight };

		float dx = bCenter.x - aCenter.x;
		float dy = bCenter.y - aCenter.y;
		float minDistX = a.halfWidth + b.halfWidth;
		float minDistY = a.halfHeight + b.halfHeight;

		if (abs(dx) > minDistX || abs(dy) > minDistY) {
			return result;
		}

		float overLapX = minDistX - abs(dx);
		float overLapY = minDistY - abs(dy);


		if (overLapX < overLapY) {
			result.normal = (dx < 0) ? SDL_FPoint{ 1.0f,0.f } : SDL_FPoint{ -1.0f,0.f };
			result.overlap = { (dx < 0) ? a.rect.x + a.rect.w - b.rect.x : b.rect.x + b.rect.w - a.rect.x,
				0, overLapX, 0 };
		}
		else {
			result.normal = (dy < 0) ? SDL_FPoint{ 0.f,1.0f } : SDL_FPoint{ 0.f,-1.0f };
			result.overlap = { 0, (dy < 0) ? a.rect.y + a.rect.h - b.rect.y : b.rect.y + b.rect.h - a.rect.y,
			0.f,overLapY };
		}
		result.isColliding = true;
		return result;
	}
	static void resolveOverlap(SDL_FRect& overlapper, const SDL_FRect& overlapData, const SDL_FPoint& normal) {
		overlapper.x += normal.x * overlapData.w;
		overlapper.y += normal.y * overlapData.h;
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

struct EntityData {
	EntityGeneric id = 0;
	RectTransform transform;
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
	AnimationController animControlls;

	EntityState state;
	RectTransform transform;


	PlayerID id = 0;
	float healthPoints = 0;

	Player(SDL_Texture* texture, const std::map<AnimID, AnimationReel>& animationData, const EntityData& data):texture(texture), animControlls(animationData) {
		id = data.id;
		textureSrc = { 0,0,data.frameWidth, data.frameHeight };
		textureDest = { 0,0,data.frameWidth, data.frameHeight };

		transform = data.transform;
		healthPoints = data.health_points;
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

	EntityState state;
	RectTransform transform;

	EnemyID id = 0;
	float health_points = 0.f;
	EnemyBasic(SDL_Texture* texture, const EntityData& data):texture(texture) {
		id = data.id;
		textureSrc = { 0,0,data.frameWidth, data.frameHeight };
		textureDest = { 0,0,data.frameWidth, data.frameHeight };

		transform = data.transform;
		health_points = data.health_points;
	}
};
