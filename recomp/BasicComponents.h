#pragma once

#include "SDL3/SDL.h"
#include <map>
#include <vector>
#include <random>
#include <array>
#include "CCPRect.h"

//cool new stuff
namespace badEngine {


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

	enum MyPersonalEvents {
		MPE_cleanup = 1 << 0,
		MPE_movingObject = 1 << 1,
		MPE_checkAnimation = 1 << 2,
		MPE_checkDirection = 1 << 3,
		test = 1 << 128
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

	class RandomNumberGenerator {
		std::random_device rd;
		std::unique_ptr<std::mt19937> rng;

	public:
		RandomNumberGenerator() : rng(std::make_unique<std::mt19937>(rd())) {}

		int getRand(int min, int max);
	};

	struct EntityData {
		EntityGeneric id = 0;
		CCP::HitBox hitBox;
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

		CCP::HitBox hitBox;

		EntityEvent events;
		Direction direction = Direction::none;


		Player(SDL_Texture* texture, const std::vector<FrameMap>& frameMap, const EntityData& data) :texture(texture), animControlls(frameMap) {
			textureSrc = { 0,0,data.frameWidth, data.frameHeight };
			textureDest = { 0,0,data.frameWidth, data.frameHeight };

			hitBox = data.hitBox;
		}

		void applyCollisionBoxToRenderBox() {
			textureDest = hitBox.rectangle;
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

		CCP::HitBox hitBox;

		EnemyBasic(SDL_Texture* texture, const EntityData& data) :texture(texture) {
			textureSrc = { 0,0,data.frameWidth, data.frameHeight };
			textureDest = { 0,0,data.frameWidth, data.frameHeight };

			hitBox = data.hitBox;
		}
	};
}