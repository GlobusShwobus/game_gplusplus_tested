#include "EntityData.h"
namespace badEngine {

	namespace EDM {
		Player::Player(const TSA::Sprite& sprite, const CCP::HitBox& hitbox) :sprite(sprite), hitbox(hitbox) {}

		bool Player::WASD_PlayerVelocity(const float moveSpeed) {
			const auto* keystate = SDL_GetKeyboardState(nullptr);

			bool w = keystate[SDL_SCANCODE_W];
			bool a = keystate[SDL_SCANCODE_A];
			bool s = keystate[SDL_SCANCODE_S];
			bool d = keystate[SDL_SCANCODE_D];

			bool outcome = false;

			if (w)      { hitbox.velocity.y -= moveSpeed; outcome = true; }
			else if (a) { hitbox.velocity.x -= moveSpeed; outcome = true; }
			else if (s) { hitbox.velocity.y += moveSpeed; outcome = true; }
			else if (d) { hitbox.velocity.x += moveSpeed; outcome = true; }

			return outcome;
		}

		Enemy::Enemy(const TSA::Sprite& sprite, const CCP::HitBox& hitbox) :sprite(sprite), hitbox(hitbox) {}
	}


}