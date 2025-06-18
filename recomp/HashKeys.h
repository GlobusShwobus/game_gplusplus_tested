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

		typedef unsigned int ENTITY_TYPES_HKEY;
		constexpr ENTITY_TYPES_HKEY PLAYER_MAIN = createHashKey("player_main");
		constexpr ENTITY_TYPES_HKEY ENEMY_SPEAR1 = createHashKey("enemy_spear1");
		constexpr ENTITY_TYPES_HKEY ENEMY_SWORD1 = createHashKey("enemy_sword1");

		enum class ENTITY_TYPE {
			UNKNOWN = 0,
			PLAYER_MAIN = 1,
			ENEMY_SPEAR1 = 2,
			ENEMY_SWORD1 = 3
		};

		constexpr ENTITY_TYPE buildEntityTypeID(const char* str) {
			ENTITY_TYPE key;
			switch (createHashKey(str)) {
			case PLAYER_MAIN:  key = ENTITY_TYPE::PLAYER_MAIN;  break;
			case ENEMY_SPEAR1: key = ENTITY_TYPE::ENEMY_SPEAR1; break;
			case ENEMY_SWORD1: key = ENTITY_TYPE::ENEMY_SWORD1; break;
			default:           key = ENTITY_TYPE::UNKNOWN;      break;
			}
			return key;
		}
		constexpr bool isValidID(ENTITY_TYPE id) {
			return id != ENTITY_TYPE::UNKNOWN;
		}
	}

}