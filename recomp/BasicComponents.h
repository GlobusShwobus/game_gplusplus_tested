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
constexpr EntityType EntityType_ENEMY = HASH("player_type");
constexpr EntityType EntityType_ENEMY = HASH("enemy_type");

//####################################################################################################
//--------------------   ENTITY IDs   ----------------------------------------------------------------
//####################################################################################################
typedef HASH_ID_TYPE EntityID;
typedef EntityID EnemyID;
constexpr EnemyID EnemyID_SPEAR1 = HASH("enemy_spear1");
constexpr EnemyID EnemyID_SWORD1 = HASH("enemy_sword1");



struct AnimationReel {
	AnimID id = 0;
	SDL_FRect initialFrame;
	int frameCount = 0;
	int frameDelay = 0;
	bool isLooping = false;
};

class AnimationController {
	const std::vector<AnimationReel>* const clips = nullptr;//not owner
	const AnimationReel* currentReel = nullptr;//points to dinkelberg
	int frameIndex = 0;
	int frameTimer = 0;

public:
	AnimationController() = default;//TODO: must set clips for entities, not owner

	void update(AnimID id);
	void setNewReel(AnimID id);
	//creates a rectngle describing frame, acts as source for texture rendering
	const SDL_FRect getCurrentFrame()const;
};

class NPCState {//TODO:only useful for entities that move, item types need another one
public:
	enum class State {
		idle, walking
	};
	enum class Facing {
		up, down, left, right
	};

	//this function will call different types of state update functions based on whatever entity ower type is
	//mainly issue of player vs AI
	void update();

	State getState()const;
	Facing getFacing()const;

	NPCState(const EntityType type) :typeOfOwner(type) {}
private:
	State currentState = State::idle;
	Facing currentFacing = Facing::down;
	
	EntityType typeOfOwner = 0;

	//only really for player/or some AI script who knows
	void setOnKeyboardInput();
};

class Sprite {

	SDL_Texture* texture = nullptr;//not owner
	SDL_FRect source = { 0,0,0,0 };
	SDL_FRect destination = { 0,0,0,0 };

public:

	Sprite(SDL_Texture* Texture, SDL_FRect* src, SDL_FRect* dest) :texture(Texture), source(*src), destination(*dest) {}
	Sprite() = default;//REMEMBER TO CLEAN THIS UP WHEN DONE WITH ENTITY/COMPONENT ARCHITECTURE

	SDL_Texture* getTexture();
	SDL_FRect* getSource();
	SDL_FRect* getDestination();
};

struct EnemyData {
	EntityID id = 0;
	float movement_speed = 0.f;
	float health_points = 0.f;
	float attack_power = 0.f;
	float attack_interval = 0.f;
};


//SET UP COMPONENT MANAGMENT FIRST, THEN ENTITIES AND GAMESCENE

class Player {


	Sprite sprite;
	NPCState state;

	float movementSpeed = 0;
	float healthPoints = 0;
	float attackPower = 0;





}



class Player {

public:

	Sprite sprite;
	Movement movement;

	static constexpr float speed = 2.5f;//uhm, speed*FPS is real speed so yeah. oops

	Player(Sprite texture, SDL_FPoint cameraRadii) :sprite(texture) {}
};
