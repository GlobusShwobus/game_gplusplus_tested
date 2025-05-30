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

class Transform {
	//scale?

	SDL_Point position{ 0,0 };
	SDL_Point previousPosition{ 0,0 };
	SDL_Point size{ 0,0 };
	SDL_Point halfSize{ 0,0 };

public:
	Transform() = default;
	Transform(int x, int y, int w, int h) {
		
		position.x = x;
		position.y = y;
		previousPosition = position;
		size.x = w;
		size.y = h;
		halfSize.x = size.x / 2;
		halfSize.y = size.y / 2;
	
	}

	const SDL_Point* const getPosition() {
		return &position;
	}
	const SDL_Point* const getSize() {
		return &size;
	}

	void setPosStaight(int x, int y) {
		previousPosition = position;

		position.x = x;
		position.y = y;
	}
	void addToCurrentPosition(const SDL_Point& velocity) {
		previousPosition = position;

		position.x += velocity.x;
		position.y += velocity.y;
	}
	bool doesIntersect(const SDL_Point& oPos, const SDL_Point& oSize)const {
		return position.x < oPos.x + oSize.x && oPos.x < position.x + size.x &&
			position.y < oPos.y + oSize.y && oPos.y < position.y + size.y;
	}

	//does NOT belong here, is shit anyhow
	bool clampPosition(int x, int y, int w, int h) {
		bool isClamped = false;
		if (position.x < x) {
			position.x = x;
			isClamped = true;
		}
		if (position.y < y) {
			position.y = y;
			isClamped = true;
		}
		if (position.x + size.x > w) {
			position.x = w - size.x;
			isClamped = true;
		}
		if (position.y + size.y > h) {
			position.y = h - size.y;
			isClamped = true;
		}
		return isClamped;
	}

	void applyDestinationTexture(SDL_FRect& dest)const {
		dest.x = (float)position.x;
		dest.y = (float)position.y;
	}
};
class Physics {
	SDL_Point velocity{ 0,0 };
	float movementSpeed = 0;
	float mass = 0;
public:
	Physics() = default;
	Physics(float movementSpeed, float mass) :movementSpeed(movementSpeed), mass(mass) {}

	void setVelocity(int x, int y) {
		velocity.x = x;
		velocity.y = y;
	}
	void setVelocity(const SDL_Point& velocity) {
		this->velocity = velocity;
	}
	void reverseVelocity() {
		velocity.x *= -1;
		velocity.y *= -1;
	}

	const SDL_Point& getVelocity() {
		return velocity;
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
	Physics physics;

	PlayerID id = 0;
	float healthPoints = 0;

	Player(SDL_Texture* texture, const std::map<AnimID, AnimationReel>& animationData, const EntityData& data):texture(texture), animControlls(animationData) {
		id = data.id;
		textureSrc = { 0,0,data.frameWidth, data.frameHeight };
		textureDest = { 0,0,data.frameWidth, data.frameHeight };

		transform = data.transform;
		physics = Physics(data.movement_speed, data.mass);
		healthPoints = data.health_points;
	}
};

class EnemyBasic {
public:
	SDL_Texture* texture = nullptr;//not owner
	SDL_FRect textureSrc{ 0,0,0,0 };
	SDL_FRect textureDest{ 0,0,0,0 };

	EntityState state;
	Transform transform;
	Physics physics;

	EnemyID id = 0;
	float health_points = 0.f;
	EnemyBasic(SDL_Texture* texture, const EntityData& data):texture(texture) {
		id = data.id;
		textureSrc = { 0,0,data.frameWidth, data.frameHeight };
		textureDest = { 0,0,data.frameWidth, data.frameHeight };

		transform = data.transform;
		physics = Physics(data.movement_speed, data.mass);
		health_points = data.health_points;
	}
};
