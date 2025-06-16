#pragma once

namespace badEngine {

	//hash keys
	namespace HKey {
		constexpr unsigned int createHashKey(const char* str) {
			unsigned int hash = 2166136261u;
			while (*str) {
				hash ^= *str++;
				hash *= 16777619u;
			}
			return hash;
		}

		typedef unsigned int ANIMATION_HKEY;
		constexpr ANIMATION_HKEY ANIMATION_WALK_LEFT = createHashKey("walk_left");
		constexpr ANIMATION_HKEY ANIMATION_WALK_RIGHT   = createHashKey("walk_right");
		constexpr ANIMATION_HKEY ANIMATION_IDLE_LEFT = createHashKey("idle_left");
		constexpr ANIMATION_HKEY ANIMATION_IDLE_RIGHT = createHashKey("idle_right");
		
		enum class ANIMATION_ENUM_KEY {
			UNKNOWN = 0,
			WALK_LEFT = 1,
			WALK_RIGHT = 2,
			IDLE_LEFT = 3,
			IDLE_RIGHT = 4
		};
		constexpr ANIMATION_ENUM_KEY buildAnimationID(const char* str) {
			ANIMATION_ENUM_KEY key;

			switch (createHashKey(str)) {
			case ANIMATION_WALK_LEFT:   key = ANIMATION_ENUM_KEY::WALK_LEFT;   break;
			case ANIMATION_WALK_RIGHT:  key = ANIMATION_ENUM_KEY::WALK_RIGHT;  break;
			case ANIMATION_IDLE_LEFT:   key = ANIMATION_ENUM_KEY::IDLE_LEFT;   break;
			case ANIMATION_IDLE_RIGHT:  key = ANIMATION_ENUM_KEY::IDLE_RIGHT;  break;
			default:                    key = ANIMATION_ENUM_KEY::UNKNOWN;	   break;
			}

			return key;
		}
		constexpr bool isValidID(ANIMATION_ENUM_KEY id) {
			return id != ANIMATION_ENUM_KEY::UNKNOWN;
		}

		typedef unsigned int ENTITY_CATEGORY_HKEY;
		constexpr ENTITY_CATEGORY_HKEY ENTITY_TYPE_PLAYER = createHashKey("player_type");
		constexpr ENTITY_CATEGORY_HKEY ENTITY_TYPE_ENEMY  = createHashKey("enemy_type");

		enum class ENTITY_CATEGORY_ENUM_KEY {
			UNKNOWN = 0,
			PLAYER = 1,
			ENEMY = 2,
		};
		constexpr ENTITY_CATEGORY_ENUM_KEY buildCategoryID(const char* str) {
			ENTITY_CATEGORY_ENUM_KEY key;
			
			switch (createHashKey(str)) {
			case ENTITY_TYPE_PLAYER:    key = ENTITY_CATEGORY_ENUM_KEY::PLAYER;   break;
			case ENTITY_TYPE_ENEMY:     key = ENTITY_CATEGORY_ENUM_KEY::ENEMY;    break;
			default:                    key = ENTITY_CATEGORY_ENUM_KEY::UNKNOWN;  break;
			}

			return key;
		}
		constexpr bool isValidID(ENTITY_CATEGORY_ENUM_KEY id) {
			return id != ENTITY_CATEGORY_ENUM_KEY::UNKNOWN;
		}

		typedef unsigned int PLAYER_ID_HKEY;
		constexpr PLAYER_ID_HKEY PLAYER_MAIN = createHashKey("player_main");
		enum class PLAYER_ENUM_KEY {
			UNKNOWN = 0,
			PLAYER_MAIN = 1,
		};
		constexpr PLAYER_ENUM_KEY buildPlayerTypeID(const char* str) {
			PLAYER_ENUM_KEY key;
			switch (createHashKey(str)) {
			case PLAYER_MAIN: key = PLAYER_ENUM_KEY::PLAYER_MAIN; break;
			default:          key = PLAYER_ENUM_KEY::UNKNOWN;     break;
			}
			return key;
		}
		constexpr bool isValidID(PLAYER_ENUM_KEY id) {
			return id != PLAYER_ENUM_KEY::UNKNOWN;
		}

		typedef unsigned int ENEMY_ID_HKEY;
		constexpr ENEMY_ID_HKEY ENEMY_SPEAR1 = createHashKey("enemy_spear1");
		constexpr ENEMY_ID_HKEY ENEMY_SWORD1 = createHashKey("enemy_sword1");

		enum class ENEMY_ENUM_KEY {
			UNKNOWN = 0,
			ENEMY_SPEAR1 = 1,
			ENEMY_SWORD1 = 2
		};

		constexpr ENEMY_ENUM_KEY buildEnemyTypeID(const char* str) {
			ENEMY_ENUM_KEY key;
			switch (createHashKey(str)) {
			case ENEMY_SPEAR1: key = ENEMY_ENUM_KEY::ENEMY_SPEAR1; break;
			case ENEMY_SWORD1: key = ENEMY_ENUM_KEY::ENEMY_SWORD1; break;
			default:           key = ENEMY_ENUM_KEY::UNKNOWN;      break;
			}
			return key;
		}
		constexpr bool isValidID(ENEMY_ENUM_KEY id) {
			return id != ENEMY_ENUM_KEY::UNKNOWN;
		}
	}

}