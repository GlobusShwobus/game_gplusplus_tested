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
constexpr EntityType EntityType_PLAYER = HASH("player_type");
constexpr EntityType EntityType_ENEMY = HASH("enemy_type");

//####################################################################################################
//--------------------   ENTITY IDs   ----------------------------------------------------------------
//####################################################################################################
typedef HASH_ID_TYPE EntityID;
typedef EntityID EnemyID;
typedef EntityID PlayerID;

constexpr EnemyID EnemyID_SPEAR1 = HASH("enemy_spear1");
constexpr EnemyID EnemyID_SWORD1 = HASH("enemy_sword1");

constexpr PlayerID PlayerID_Version1 = HASH("player_version1");









struct AnimationReel {
	AnimID id = 0;
	SDL_FRect initialFrame;
	int frameCount = 0;
	int frameDelay = 0;
	bool isLooping = false;
};

struct TextureData {

	SDL_Texture* texture = nullptr;//not owner
	SDL_FRect source = { 0,0,0,0 };
	SDL_FRect destination = { 0,0,0,0 };

	TextureData(SDL_Texture* Texture, SDL_FRect* src, SDL_FRect* dest) :texture(Texture), source(*src), destination(*dest) {}
	TextureData() = default;//Entity factory map does not like without default constructor, need to do something about it
};

class AnimationController {
	const std::vector<AnimationReel>* const clips = nullptr;//not owner
	const AnimationReel* currentReel = nullptr;//points to dinkelberg
	int frameIndex = 0;
	int frameTimer = 0;

public:
	//reels originates from EntityFactory witch holds the data, it is always valid and should not be freed
	AnimationController(const std::vector<AnimationReel>* const reels) :clips(reels), currentReel(&reels->front()) {}

	void moveFrame();
	void setNewReel(AnimID id);
	//creates a rectngle describing frame, acts as source for texture rendering
	void applySourceFromFrame(SDL_FRect* const rect)const;
};

class NPCState {//TODO:only useful for entities that move, item types need another one
public:
	enum class State {
		idle, moving
	};
	enum class Facing {
		up, down, left, right
	};
	//should be run each frame

	void setState(const State state);
	void setFacing(const Facing facing);

	State getState()const;
	Facing getFacing()const;

	bool didChangeOccur()const;
	void handeledChange();
private:
	bool wasChange = false;
	State currentState = State::idle;
	Facing currentFacing = Facing::down;
};

class Transform {
	//scale?

	SDL_Point position{ 0,0 };
	SDL_Point previousPosition{ 0,0 };
	SDL_Point size{ 0,0 };
	SDL_Point halfSize{ 0,0 };
	SDL_FPoint velocity{ 0.f,0.f };

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

	const SDL_Point* const getCurrentPos() {
		return &position;
	}
	const SDL_Point* const getSize() {
		return &size;
	}

};


struct EnemyData {
	EntityID id = 0;
	float movement_speed = 0.f;
	float health_points = 0.f;
	float attack_power = 0.f;
	float attack_interval = 0.f;
};

struct PlayerData {
	PlayerID id = 0;
	Transform transform;
	float movement_speed = 0.f;
	float health_points = 0.f;
	float attack_power = 0.f;
};


class Player {

public:

	TextureData texture;
	NPCState state;
	AnimationController animControlls;
	Transform transform;

	PlayerID id = 0;
	float movementSpeed = 0;
	float healthPoints = 0;
	float attackPower = 0;

	Player(const TextureData* const sprite, const std::vector<AnimationReel>* const reels, const PlayerData* const data):texture(*sprite), animControlls(reels) {
		id = data->id;
		transform = data->transform;
		movementSpeed = data->movement_speed;
		healthPoints = data->health_points;
		attackPower = data->attack_power;


		const SDL_Point* const pos = transform.getCurrentPos();
		const SDL_Point* const size = transform.getSize();

		SDL_FRect dest{ pos->x, pos->y, size->x, size->y };//currently the texutre/frame size is same as collision box size, but now we can mix and match

		this->texture.destination = dest;
	}
};
