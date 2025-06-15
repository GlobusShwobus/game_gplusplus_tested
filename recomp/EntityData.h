#pragma once

#include "SDL3/SDL.h"
#include "CCPRect.h"
#include "HashKeys.h"
#include "Sprite.h"

//cool new stuff
namespace badEngine {

	//entity data manager
	namespace EDM {

		enum class PlayerType {
			PLAYER
		};
		enum class EnemyType {
			ENEMY_SPEAR1,
			ENEMY_SWORD1
		};

		class Player {

		public:

			TSA::Sprite sprite;
			CCP::HitBox hitbox;

			Player(const TSA::Sprite& sprite, const CCP::HitBox& hitbox);

			void updateHitBoxToSourceTexture();
			bool WASD_PlayerVelocity(SDL_FPoint& velocity, const float moveSpeed);
		};

		class EnemyBasic {
		public:

			TSA::Sprite sprite;
			CCP::HitBox hitbox;

			EnemyBasic(const TSA::Sprite& sprite, const CCP::HitBox& hitbox);
		};

	}
}