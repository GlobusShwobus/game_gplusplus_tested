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
			SDL_Texture* texture = nullptr;
			SDL_FRect source{ 0,0,0,0 };
			SDL_FRect dest  { 0,0,0,0 };


			static constexpr int frameDelay = 6;
			std::vector<Reel>* clips = nullptr;
			const Reel* current = nullptr;

		public:

			bool initPlay(std::vector<Reel>* animationPoints) {
				if (!animationPoints) return false;
				if (animationPoints->empty()) return false;

				clips = animationPoints;
				current = &clips->front();

				return true;
			}
			void play();
			bool setNewAnimation(const AnimationIDs id);
			void updateSource();

		};

		struct FrameMap {
			AnimationIDs id = AnimationIDs::DEFAULT;
			std::vector<SDL_FRect> frames;
			bool isLooping = false;
		};

		class Animation {
			static constexpr int frameDelay = 6;

			const std::vector<FrameMap>& clips;
			const FrameMap* current = nullptr;

			int frameIndex = 0;
			int frameTimer = 0;
		public:
			Animation(const std::vector<FrameMap>& reels);
			void moveFrame();
			bool setIfNew(const AnimationIDs id);

			SDL_FRect getCurrentFrameRect();
		};


		AnimationIDs animationIDTable(const StateM::State& state);
	}



}