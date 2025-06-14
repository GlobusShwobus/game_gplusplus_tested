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
		constexpr ANIMATION_HKEY ANIMATION_IDLE_FACING_LEFT = createHashKey("idle_left");
		constexpr ANIMATION_HKEY ANIMATION_IDLE_FACING_RIGHT = createHashKey("idle_right");

		typedef unsigned int ENTITY_CATEGORY_HKEY;
		constexpr ENTITY_CATEGORY_HKEY ENTITY_TYPE_PLAYER = createHashKey("player_type");
		constexpr ENTITY_CATEGORY_HKEY ENTITY_TYPE_ENEMY  = createHashKey("enemy_type");

		constexpr bool definedEntityType(ENTITY_CATEGORY_HKEY x) {
			return x == ENTITY_TYPE_PLAYER || x == ENTITY_TYPE_ENEMY;
		}

	}

}