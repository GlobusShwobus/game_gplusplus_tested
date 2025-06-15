#include "Sprite.h"

namespace badEngine {

	namespace TSA {
		bool Sprite::initPlay(std::vector<Reel>* animationPoints) {
			if (!animationPoints) return false;
			if (animationPoints->empty()) return false;

			animations = animationPoints;
			current = &animations->front();

			return true;
		}
		void Sprite::unInitPlay() {
			current = nullptr;
			animations = nullptr;
		}
		bool Sprite::play() {
			if (!animations) return false;

			frameTimer++;
			if (frameTimer < frameDelay) return false;
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
			return true;
		}
		bool Sprite::setNewAnimation(const AnimationIDs id) {
			if (!animations)return false;
			if (current->id == id) return false;//early out if same animation

			for (auto& each : *animations) {
				if (each.id == id) {
					current = &each;
					frameIndex = 0;
					return true;
				}
			}
			return false;
		}
		void Sprite::updateSource() {
			if (!animations)return;

			source.x = current->frames[frameIndex].x;
			source.y = current->frames[frameIndex].y;
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
