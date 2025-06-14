#include "AnimationHandler.h"

namespace badEngine {

	namespace AHC {
		Animation::Animation(const std::vector<FrameMap>& reels) :clips(reels) {
			if (!reels.empty()) {
				current = &reels.front();
			}
		}
		void Animation::moveFrame() {
			frameTimer++;
			if (frameTimer < frameDelay)return;
			frameTimer = 0;
			frameIndex++;

			if (current->isLooping) {
				if (frameIndex >= current->frames.size())
					frameIndex = 0;
			}
			else {
				if (frameIndex >= current->frames.size())
					frameIndex = current->frames.size() - 1;
			}
		}
		bool Animation::setIfNew(const AnimationIDs id) {
			if (current->id == id) return false;//early out if same animation

			for (auto& each : clips) {
				if (each.id == id) {
					current = &each;
					frameIndex = 0;
					return true;
				}
			}
			return false;
		}
		SDL_FRect Animation::getCurrentFrameRect() {
			return current->frames[frameIndex];
		}

		AnimationIDs animationIDTable(const StateM::State& state) {
			AnimationIDs id = AnimationIDs::DEFAULT;
			if (state.isMoving) {
				if (state.isFacingLeft) {
					id = AnimationIDs::WALK_LEFT;
				}
				else {
					id = AnimationIDs::WALK_RIGHT;
				}
			}
			else {
				if (state.isFacingLeft) {
					id = AnimationIDs::IDLE_LEFT;
				}
				else {
					id = AnimationIDs::IDLE_RIGHT;
				}
			}
			return id;
		}
	}

}
