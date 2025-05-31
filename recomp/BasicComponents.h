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
	none, left, right, up, down
};
enum EntityEvents {
	noEvent = 1 << 0,
	directionChange = 1 << 1,
	collsion_fromUp = 1 << 2,
	collision_fromDown = 1 << 3,
	collision_fromLeft = 1 << 4,
	collision_fromRight = 1 << 5,
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
	SDL_Rect BB{ 0,0,0,0 };
	SDL_Point prevPos{ 0,0 };

	SDL_Point velocity{ 0,0 };

	SDL_Point halfSize{ 0,0 };
public:
	Transform() = default;
	Transform(int x, int y, int w, int h) {
		BB = { x,y,w,h };

		prevPos.x = x;
		prevPos.y = y;

		halfSize.x = w / 2;
		halfSize.y = w / 2;
	}

	const SDL_Rect& getBB()const {
		return BB;
	}

	void setPosRaw(int x, int y) {
		prevPos.x = BB.x;
		prevPos.y = BB.y;

		BB.x = x;
		BB.y = y;
	}
	void updatePosition() {
		prevPos.x = BB.x;
		prevPos.y = BB.y;

		BB.x += velocity.x;
		BB.y += velocity.y;
	}
	void setVelocity(int x, int y) {
		velocity.x = x;
		velocity.y = y;
	}

	//checks if this obj contains another whole
	bool contains(const SDL_Rect& r)const {
		return(BB.x <= r.x) && (r.x + r.w < BB.x + BB.w) &&
			(BB.y <= r.y) && (r.y + r.h < BB.y + BB.h);
	}
	//checks if two rects overlap
	bool overlaps(const SDL_Rect& r)const {
		return BB.x < r.x + r.w && r.x < BB.x + BB.w &&
			BB.y < r.y + r.h && r.y < BB.y + BB.h;
	}
	//reflects velocity
	void reflectVelocity(const SDL_Rect& r) {
		if (BB.x<r.x || BB.x + BB.w>r.x + r.w) {
			velocity.x *= -1;
		}
		if (BB.y<r.y || BB.y + BB.h>r.y + r.h) {
			velocity.y *= -1;
		}
	}

	//MOVE THIS LATER SOMWHERE ELSE
	void clampInsideOf(const SDL_Rect& object) {
		if (BB.x < object.x) {
			BB.x = object.x;
		}
		if (BB.y < object.y) {
			BB.y = object.y;
		}
		if (BB.x + BB.w > object.w) {
			BB.x = object.w - BB.w;
		}
		if (BB.y + BB.h > object.h) {
			BB.y = object.h - BB.h;
		}
	}
	//MOVE THIS LATER SOMWHERE ELSE
	void applyDestinationTexture(SDL_FRect& dest)const {
		dest.x = (float)BB.x;
		dest.y = (float)BB.y;
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
	float movementSpeed = 0;
	float mass = 0;


	PlayerID id = 0;
	float healthPoints = 0;

	Player(SDL_Texture* texture, const std::map<AnimID, AnimationReel>& animationData, const EntityData& data):texture(texture), animControlls(animationData) {
		id = data.id;
		textureSrc = { 0,0,data.frameWidth, data.frameHeight };
		textureDest = { 0,0,data.frameWidth, data.frameHeight };

		transform = data.transform;
		movementSpeed = data.movement_speed;
		mass = data.mass;
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
	float movementSpeed = 0;
	float mass = 0;

	EnemyID id = 0;
	float health_points = 0.f;
	EnemyBasic(SDL_Texture* texture, const EntityData& data):texture(texture) {
		id = data.id;
		textureSrc = { 0,0,data.frameWidth, data.frameHeight };
		textureDest = { 0,0,data.frameWidth, data.frameHeight };

		transform = data.transform;
		movementSpeed = data.movement_speed;
		mass = data.mass;
		health_points = data.health_points;
	}
};
