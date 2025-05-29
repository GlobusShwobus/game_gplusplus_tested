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

class NPCState {//TODO:only useful for entities that move, item types need another one
public:
	enum class State {
		idle, moving
	};
	enum class Direction {
		up, down, left, right
	};
	void setState(const State state);
	void setDirection(const Direction facing);

	State getState()const;
	Direction getDirection()const;

	bool didChangeOccur()const;
	void handeledChange();
private:
	bool wasChange = false;
	State currentState = State::idle;
	Direction currentDirection = Direction::down;
};

class Transform {
	//scale?

	SDL_Point position{ 0,0 };
	SDL_Point previousPosition{ 0,0 };
	SDL_Point size{ 0,0 };
	SDL_Point halfSize{ 0,0 };
	SDL_Point velocity{ 0,0 };

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

	void updatePosition() {
		previousPosition = position;

		position.x += velocity.x;
		position.y += velocity.y;
	}
	//maybe temp, will see
	void clampPosition(int x, int y, int w, int h) {
		if (position.x < x) {
			position.x = x;
		}
		if (position.y < y) {
			position.y = y;
		}
		if (position.x + size.x > w) {
			position.x = w - size.x;
		}
		if (position.y + size.y > h) {
			position.y = h - size.y;
		}
	}
	void incrementVelocity(NPCState::Direction dir, const float moveSpeed) {
		switch (dir) {
		case NPCState::Direction::up:    velocity.y -= (int)moveSpeed; break;
		case NPCState::Direction::down:  velocity.y += (int)moveSpeed; break;
		case NPCState::Direction::left:  velocity.x -= (int)moveSpeed; break;
		case NPCState::Direction::right: velocity.x += (int)moveSpeed; break;
		default://no change but other dirs should not exist (maybe diagonals later)
			break;
		}
	}
	void resetVelocity() {
		velocity.x = 0;
		velocity.y = 0;
	}
	void applyDestinationTexture(SDL_FRect& dest)const {
		dest.x = (float)position.x;
		dest.y = (float)position.y;
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

	NPCState state;

	Transform transform;

	PlayerID id = 0;
	float movementSpeed = 0;
	float healthPoints = 0;
	float mass = 0;

	Player(SDL_Texture* texture, const std::map<AnimID, AnimationReel>& animationData, const EntityData& data):texture(texture), animControlls(animationData) {
		id = data.id;
		textureSrc = { 0,0,data.frameWidth, data.frameHeight };
		textureDest = { 0,0,data.frameWidth, data.frameHeight };

		transform = data.transform;
		movementSpeed = data.movement_speed;
		healthPoints = data.health_points;
		mass = data.mass;
	}
};

class EnemyBasic {
public:
	SDL_Texture* texture = nullptr;//not owner
	SDL_FRect textureSrc{ 0,0,0,0 };
	SDL_FRect textureDest{ 0,0,0,0 };

	NPCState state;
	Transform transform;

	EnemyID id = 0;
	float movement_speed = 0.f;
	float health_points = 0.f;
	float mass = 0.f;

	bool isActivated = false;

	EnemyBasic(SDL_Texture* texture, const EntityData& data):texture(texture) {
		id = data.id;
		textureSrc = { 0,0,data.frameWidth, data.frameHeight };
		textureDest = { 0,0,data.frameWidth, data.frameHeight };

		transform = data.transform;
		movement_speed = data.movement_speed;
		health_points = data.health_points;
		mass = data.mass;
		
		isActivated = true;
	}
};
