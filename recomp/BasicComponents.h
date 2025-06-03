#pragma once

#include "SDL3/SDL.h"
#include <map>
#include <vector>
#include <random>
#include <array>

#include <iostream>
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

enum Direction {
	left = 0, right = 1, up = 2, down = 3
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
	Direction direction;
public:
	EntityState() :action(EntityAction::idle), direction(Direction::down), isActive(true) {}
	void deactivate();
	bool isActivated()const;
	void changeAction(EntityAction action);
	void changeDirection(Direction direction);
	void setEvent(const EntityEvents events);
	void flushEvents();
	int getEvents()const;
	bool containsEvent(EntityEvents event)const;
	Direction getDirection()const;
	EntityAction getAction()const;
};

struct RectRayProjection {
	SDL_FPoint entry{ 0,0 };
	SDL_FPoint exit{ 0,0 };

	void projectRay(const SDL_FRect& origin, const SDL_FPoint& vel, const SDL_FRect& target) {
		SDL_FPoint inverse = { 1.0f / vel.x, 1.0f / vel.y };

		entry = { (target.x - origin.x) * inverse.x,(target.y - origin.y) * inverse.y };
		exit = { (target.x + target.w - origin.x) * inverse.x, (target.y + target.h - origin.y) * inverse.y };
	}
};
struct Transform {
	SDL_FRect rect{ 0,0,0,0 };
	SDL_FPoint velocity{ 0,0 };

	std::array<bool, 4> collisionSide{ false,false,false,false };

	Transform() = default;
	Transform(float x, float y, float w, float h) :rect{ x,y,w,h } {}

	void updatePos() {
		rect.x += velocity.x;
		rect.y += velocity.y;
		std::ranges::fill(collisionSide, false);
	}
	void setVelocity(const SDL_FPoint& someVel) {
		velocity.x = someVel.x;
		velocity.y = someVel.y;
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

	bool projectionHitDetect(RectRayProjection& proj, float& hitTimeEntry) {

		if (std::isnan(proj.entry.x) || std::isnan(proj.entry.y) || std::isnan(proj.exit.x) || std::isnan(proj.exit.y))
			return false;

		if (proj.entry.x > proj.exit.x) std::swap(proj.entry.x, proj.exit.x);//sort if from otherside
		if (proj.entry.y > proj.exit.y) std::swap(proj.entry.y, proj.exit.y);//sort if from otherside

		if (proj.entry.x > proj.exit.y || proj.entry.y > proj.exit.x) return false;//if ray does not penetrate

		hitTimeEntry = std::max(proj.entry.x, proj.entry.y);//closest hit time
		float tHitFar = std::min(proj.exit.x, proj.exit.y);//furthest hit time

		if (tHitFar < 0) return false; //if ray is pointing away

		return true;
	}
	bool projectionHitBoxAdjusted(const SDL_FRect& target, RectRayProjection& projection, SDL_FPoint& contactPoint, float& hitTimeEntry) {
		if (velocity.x == 0 && velocity.y == 0) return false;//no movement so nothing

		SDL_FRect expandedTarget = { // need to expand the target to get a prediction 
			target.x - rect.w * 0.5f,
			target.y - rect.h * 0.5f,
			target.w + rect.w,
			target.h + rect.h
		};

		SDL_FRect originAsPoint = { rect.x + rect.w * 0.5f, rect.y + rect.h * 0.5f, 0, 0 };// center point of the origin rect

		projection.projectRay(originAsPoint, velocity, expandedTarget);//applies values to entry and exit points
		if (projectionHitDetect(projection, hitTimeEntry) && hitTimeEntry <= 1.0f) {
			contactPoint = { originAsPoint.x + hitTimeEntry * velocity.x, originAsPoint.y + hitTimeEntry * velocity.y };//if we hit, aquire the contact point
			return true;
		}
		return false;
	}

	bool projectionFirstResolution(const SDL_FRect& target, SDL_FPoint& contactPoint){
		RectRayProjection proj;
		float contactTime = 0;
		if (projectionHitBoxAdjusted(target, proj, contactPoint, contactTime)) {
			
			SDL_FPoint normal{ 0,0 };
			if (proj.entry.x > proj.entry.y) {
				if (velocity.x < 0) {
					collisionSide[(int)right] = true;
					normal = { 1,0 };
				}
				else {
					collisionSide[(int)left] = true;
					normal = { -1,0 };
				}
			}
			else if (proj.entry.x < proj.entry.y) {
				if (velocity.y < 0) {
					collisionSide[(int)down] = true;
					normal = { 0,1 };
				}
				else {
					collisionSide[(int)up] = true;
					normal = { 0,-1 };
				}
			}
			velocity.x += normal.x * std::fabs(velocity.x) * (1 - contactTime);
			velocity.y += normal.y * std::fabs(velocity.y) * (1 - contactTime);

			std::cout << "\nContactTime: " << contactTime << "\n";
			std::cout << "EntryX: " << proj.entry.x << " EntryY: " << proj.entry.y << "\n";
			std::cout << "Normal: " << normal.x << ", " << normal.y << "\n";
			std::cout << "Velocity: " << velocity.x << ", " << velocity.y << "\n";
			std::cout << "PlayerPos: " << rect.x << ", " << rect.y << "\n";
			std::cout << "Target: " << target.x << ", " << target.y << ", w=" << target.w << ", h=" << target.h << "\n";

			return true;
		}
		return false;
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
	AnimationController animControlls;

	EntityState state;
	Transform transform;


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
	Transform transform;

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
