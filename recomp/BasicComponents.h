#pragma once

#include "SDL3/SDL.h"
#include <map>
#include <vector>
#include <random>
#include <array>
#include "CCPRect.h"

//cool new stuff
namespace badEngine {

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