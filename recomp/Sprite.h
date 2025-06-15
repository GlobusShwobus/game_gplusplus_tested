#pragma once

#include <vector>
#include "HashKeys.h"
#include "SDL3/SDL.h"
#include "State.h"

namespace badEngine {

	//texture sprite animation
	namespace TSA {

		enum class AnimationIDs {
			DEFAULT = 0,
			WALK_LEFT = HKey::ANIMATION_WALK_LEFT,
			WALK_RIGHT = HKey::ANIMATION_WALK_RIGHT,
			IDLE_LEFT = HKey::ANIMATION_IDLE_FACING_LEFT,
			IDLE_RIGHT = HKey::ANIMATION_IDLE_FACING_RIGHT
		};

		struct Reel {
			AnimationIDs id = AnimationIDs::DEFAULT;
			std::vector<SDL_FPoint> frames;
			bool isLooping = false;
		};

		class Sprite {
		
		public:
			SDL_Texture* texture = nullptr;//shallow copy is OK, not owner
			SDL_FRect source{ 0,0,0,0 };
			SDL_FRect dest{ 0,0,0,0 };
		
		private:
			static constexpr int frameDelay = 6;
			std::vector<Reel>* animations = nullptr;//shallow copy is OK, not owner
			const Reel* current = nullptr;//shallow copy is OK, not owner
			int frameIndex = 0;
			int frameTimer = 0;

		public:	
			Sprite(SDL_Texture* texture, const SDL_FRect& src, const SDL_FRect& dest) :texture(texture), source(src), dest(dest) {}

			bool initPlay(std::vector<Reel>* animationPoints);
			void unInitPlay();
			bool play();
			bool setNewAnimation(const AnimationIDs id);
			void updateSource();

		};

		AnimationIDs animationIDTable(const StateM::State& state);
	}

}