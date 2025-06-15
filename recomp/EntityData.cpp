#include "EntityData.h"
namespace badEngine {

	namespace EDM {
		Player::Player(const TSA::Sprite& sprite, const CCP::HitBox& hitbox) :sprite(sprite), hitbox(hitbox) {}

		void Player::updateHitBoxToSourceTexture() {
			sprite.dest.x = hitbox.rectangle.x;
			sprite.dest.y = hitbox.rectangle.y;
		}
		bool Player::WASD_PlayerVelocity(SDL_FPoint& velocity, const float moveSpeed) {
			const auto* keystate = SDL_GetKeyboardState(nullptr);

			bool w = keystate[SDL_SCANCODE_W];
			bool a = keystate[SDL_SCANCODE_A];
			bool s = keystate[SDL_SCANCODE_S];
			bool d = keystate[SDL_SCANCODE_D];

			bool outcome = false;

			if (w) { velocity.y -= moveSpeed; outcome = true; }
			else if (a) { velocity.x -= moveSpeed; outcome = true; }
			else if (s) { velocity.y += moveSpeed; outcome = true; }
			else if (d) { velocity.x += moveSpeed; outcome = true; }

			return outcome;
		}

		EnemyBasic::EnemyBasic(const TSA::Sprite& sprite, const CCP::HitBox& hitbox) :sprite(sprite), hitbox(hitbox) {}
	}


}