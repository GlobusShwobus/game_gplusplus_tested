#include "Sprite.h"

namespace badEngine {

	namespace TSA {
		bool Sprite::initPlay(const std::vector<Reel>* animationPoints)
		{
			if (!animationPoints) return false;
			if (animationPoints->empty()) return false;

			animations = animationPoints;
			current = &animations->front();

			return true;
		}
		void Sprite::unInitPlay()
		{
			current = nullptr;
			animations = nullptr;
		}
		bool Sprite::play()
		{
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
		bool Sprite::setNewAnimation(const AnimationID id)
		{
			if (!animations)return false;

			for (auto& each : *animations) {
				if (each.id == id) {
					current = &each;
					frameIndex = 0;
					return true;
				}
			}
			return false;
		}
		SDL_FRect Sprite::getAnimatedTextureSource()
		{
			if (!animations) return source;

			return { current->frames[frameIndex].x,current->frames[frameIndex].y, source.w, source.h };
		}
		AnimationID Sprite::getCurrentAnimationID()
		{
			AnimationID id = AnimationID::UNKNOWN;
			if (current)
				id = current->id;
			return id;
		}
		AnimationID animationIDTable(const SMS::State& state)
		{
			AnimationID id = AnimationID::UNKNOWN;

			if (state.isMoving) {
				if (state.facing == SMS::Facing::LEFT) {
					id = AnimationID::WALK_LEFT;
				}
				else if(state.facing == SMS::Facing::RIGHT) {
					id = AnimationID::WALK_RIGHT;
				}
			}
			else {
				if (state.facing == SMS::Facing::LEFT) {
					id = AnimationID::IDLE_LEFT;
				}
				else if(state.facing == SMS::Facing::RIGHT){
					id = AnimationID::IDLE_RIGHT;
				}
			}
			return id;
		}
		void setTTransferField_coordinates(const SDL_FRect& origin, SDL_FRect& dest)
		{
			dest.x = origin.x;
			dest.y = origin.y;
		}
		void setTTransferField_full(const SDL_FRect& origin, SDL_FRect& dest)
		{
			dest.x = origin.x;
			dest.y = origin.y;
			dest.w = origin.w;
			dest.h = origin.h;
		}

	}

}
