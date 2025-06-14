#pragma once

#include "SDL3/SDL.h"
#include "CCPRect.h"
#include "HashKeys.h"
#include "AnimationHandler.h"

//cool new stuff
namespace badEngine {

	//entity data manager
	namespace EDM {

		enum class EntityCategoryType {
			PLAYER = HKey::ENTITY_TYPE_PLAYER,
			ENEMY = HKey::ENTITY_TYPE_ENEMY,
		};
		enum class PlayerType {
			PLAYER
		};
		enum class EnemyType {
			ENEMY_SPEAR1,
			ENEMY_SWORD1
		};

		struct DynamicEntityTempalte {
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
			bool WASD_PlayerVelocity(SDL_FPoint& velocity, const float moveSpeed) {
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
}