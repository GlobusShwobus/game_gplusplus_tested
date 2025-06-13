#pragma once

#include <vector>
#include "HashKeys.h"
#include "SDL3/SDL.h"
#include "State.h"

namespace badEngine {

	//animation handling controlls
	namespace AHC {

		enum class AnimationIDs {
			DEFAULT = 0,
			WALK_LEFT = HKey::ANIMATION_WALK_LEFT,
			WALK_RIGHT = HKey::ANIMATION_WALK_RIGHT,
			IDLE_LEFT = HKey::ANIMATION_IDLE_FACING_LEFT,
			IDLE_RIGHT = HKey::ANIMATION_IDLE_FACING_RIGHT
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


		AnimationIDs animationIDTable(const StateM::State& state) {
			if (state.isMoving) {
				if (state.isFacingLeft) {

				}
				else {

				}
			}
			else {

			}
		}


	}



}