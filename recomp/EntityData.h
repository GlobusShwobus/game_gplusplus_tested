#pragma once

#include "SDL3/SDL.h"
#include "CCPRect.h"
#include "Sprite.h"
#include "HashKeys.h"
#include "State.h"

//cool new stuff
namespace badEngine {

	//entity data manager
	namespace EDM {

		class Player {

		public:
			
			TSA::Sprite sprite;
			CCP::HitBox hitbox;
			SMS::State state;

			Player(const TSA::Sprite& sprite, const CCP::HitBox& hitbox);

			bool WASD_PlayerVelocity(const float moveSpeed);
		};


		class Enemy {
		public:

			TSA::Sprite sprite;
			CCP::HitBox hitbox;

			Enemy(const TSA::Sprite& sprite, const CCP::HitBox& hitbox);
		};
		static void setCoordinates(CCP::HitBox& hitbox) {
			hitbox.rectangle.x += hitbox.velocity.x;
			hitbox.rectangle.y += hitbox.velocity.y;
		}
		static SMS::Facing facingDirection(const SDL_FPoint& velocity) {
			SMS::Facing f = SMS::Facing::UNKNOWN;
			if (velocity.x < 0)
				f = SMS::Facing::LEFT;
			else if (velocity.x > 0)
				f = SMS::Facing::RIGHT;
			return f;
		}
		static bool isMoving(const SDL_FPoint& velocity) {
			return velocity.x != 0.0f || velocity.y != 0.0f;
		}

	}
}