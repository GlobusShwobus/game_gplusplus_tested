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


struct CollisionSweptResult {
	SDL_FPoint contactPoint{ 0.f,0.f };
	SDL_FPoint normal{ 0.f,0.f };
	float tHitNear = 0.f;
};
class RectTransform {

	SDL_FRect rect{ 0,0,0,0 };
	SDL_FPoint velocity{ 0,0 };
	float halfWidth = 0.f;
	float halfHeight = 0.f;

	bool sweptAABB_unadjusted(const SDL_FRect& originPos, const SDL_FPoint& originVel, const SDL_FRect& target, CollisionSweptResult& result) {
		SDL_FPoint tEntry{ (target.x - originPos.x) / originVel.x,(target.y - originPos.y) / originVel.y };
		SDL_FPoint tExit{ (target.x + target.w - originPos.x) / originVel.x, (target.y + target.h - originPos.y) / originVel.y };

		if (tEntry.x > tExit.x) std::swap(tEntry.x, tExit.x);
		if (tEntry.y > tExit.y) std::swap(tEntry.y, tExit.y);

		if (tEntry.x > tExit.y || tEntry.y > tExit.x) return false;//didn't quite understand this one

		result.tHitNear = std::max(tEntry.x, tEntry.y);//something something going backwards
		float tHitFar = std::min(tExit.x, tExit.y);

		if (tHitFar < 0) return false; //means ray is going opposite way

		result.contactPoint = { originPos.x + result.tHitNear * originVel.x, originPos.y + result.tHitNear * originVel.y };//really need to make some SDL shorthands as i can't use my own vec2.h

		if (tEntry.x > tEntry.y) {
			if (originVel.x < 0)
				result.normal = { 1,0 };
			else
				result.normal = { -1,0 };
		}
		else if (tEntry.x < tEntry.y) {
			if (originVel.y < 0)
				result.normal = { 0,1 };
			else
				result.normal = { 0,-1 };
		}

		return true;
	}
public:
	RectTransform() = default;
	RectTransform(float x, float y, float w, float h) :rect{ x,y,w,h }, halfWidth(w * 0.5f), halfHeight(h * 0.5f) {}
	const SDL_FRect& getRect()const {
		return rect;
	}
	void setVelocity(const SDL_FPoint& someVel) {
		velocity = someVel;
	}
	void updatePosUnrestricted() {
		rect.x += velocity.x;
		rect.y += velocity.y;
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
	bool sweptAABB_adjusted(const SDL_FRect& target, CollisionSweptResult& result) {
		if (velocity.x == 0 && velocity.y == 0)
			return false;

		SDL_FRect expandedTarget = {
			target.x - halfWidth,
			target.y - halfHeight,
			target.w + rect.w,
			target.h + rect.h
		};

		SDL_FRect originAsPoint = { rect.x + halfWidth, rect.y + halfHeight, 0, 0 };

		return sweptAABB_unadjusted(originAsPoint, velocity, expandedTarget, result) && result.tHitNear <= 1.0f;
	}
	void clampOnCollision(const CollisionSweptResult& colData) {
		rect.x += velocity.x * colData.tHitNear;
		rect.y += velocity.y * colData.tHitNear;

		static const float epsilon = 0.001f;//a small buffer zone or else it's fucky
		rect.x += colData.normal.x * epsilon;
		rect.y += colData.normal.y * epsilon;
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
		textureDest = transform.getRect();
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
