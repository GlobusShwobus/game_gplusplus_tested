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
	right=0, down=1, left=2, up = 3, none
};

struct Transform {
	SDL_FRect rect{ 0,0,0,0 };
	SDL_FPoint velocity{ 0,0 };

	Transform() = default;
	Transform(float x, float y, float w, float h) :rect{ x,y,w,h } {}

	void updatePos() {
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

	bool projectionHitDetect(const SDL_FRect& originRect, const SDL_FPoint& originVel, const SDL_FRect& target, SDL_FPoint& contactPoint, SDL_FPoint& contactNormal, float& hitTimeEntry) {

		SDL_FPoint inverse = { 1.0f / originVel.x, 1.0f / originVel.y };
		SDL_FPoint entry = { (target.x - originRect.x) * inverse.x, (target.y - originRect.y) * inverse.y };
		SDL_FPoint exit = { (target.x + target.w - originRect.x) * inverse.x, (target.y + target.h - originRect.y) * inverse.y };

		if (std::isnan(entry.x) || std::isnan(entry.y) || std::isnan(exit.x) || std::isnan(exit.y)) {return false;}

		if (entry.x > exit.x) std::swap(entry.x,exit.x);//sort if from otherside
		if (entry.y > exit.y) std::swap(entry.y,exit.y);//sort if from otherside

		if (entry.x > exit.y || entry.y > exit.x) { return false; }//if ray does not penetrate

		hitTimeEntry = std::max(entry.x, entry.y);//closest hit time
		float tHitFar = std::min(exit.x, exit.y);//furthest hit time

		if (tHitFar < 0 || hitTimeEntry < 0) { return false; } //if ray is pointing away

		//if there is a hit get the contact point and normal
		contactPoint = { originRect.x + hitTimeEntry * originVel.x, originRect.y + hitTimeEntry * originVel.y };

		if (entry.x > entry.y)
			if (inverse.x < 0)
				contactNormal = { 1, 0 };
			else
				contactNormal = { -1, 0 };
		else if (entry.x < entry.y)
			if (inverse.y < 0)
				contactNormal = { 0, 1 };
			else
				contactNormal = { 0, -1 };

		return true;
	}
	bool projectionHitBoxAdjusted(const SDL_FRect& target, SDL_FPoint& contactPoint, SDL_FPoint& contactNormal, float& hitTimeEntry) {
		if (velocity.x == 0 && velocity.y == 0) { return false; }//no movement so nothing

		SDL_FRect expandedTarget = { // need to expand the target to get a prediction 
			target.x - rect.w * 0.5f,
			target.y - rect.h * 0.5f,
			target.w + rect.w,
			target.h + rect.h
		};

		SDL_FRect originAsPoint = { rect.x + rect.w * 0.5f, rect.y + rect.h * 0.5f, 0, 0 };// center point of the origin rect

		return projectionHitDetect(originAsPoint, velocity, expandedTarget, contactPoint, contactNormal, hitTimeEntry) && hitTimeEntry <= 1.0f;
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
