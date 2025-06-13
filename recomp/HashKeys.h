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

		constexpr unsigned int ANIMATION_WALK_LEFT = createHashKey("walk_left");
		constexpr unsigned int ANIMATION_WALK_RIGHT   = createHashKey("walk_right");

		constexpr unsigned int ANIMATION_IDLE_FACING_LEFT = createHashKey("idle_left");
		constexpr unsigned int ANIMATION_IDLE_FACING_RIGHT = createHashKey("idle_right");


		constexpr unsigned int ENTITY_TYPE_PLAYER = createHashKey("player_type");
		constexpr unsigned int ENTITY_TYPE_ENEMY  = createHashKey("enemy_type");

		constexpr bool definedEntityType(unsigned int x) {
			return x == ENTITY_TYPE_PLAYER || x == ENTITY_TYPE_ENEMY;
		}

	}

}