#pragma once

#include <vector>
#include "HashKeys.h"
#include "SDL3/SDL.h"
#include "State.h"

namespace badEngine {

	//texture sprite animation
	namespace TSA {
		typedef HKey::ANIMATION_ENUM_KEY AnimationID;

		struct Reel {
			AnimationID id = AnimationID::UNKNOWN;
			std::vector<SDL_FPoint> frames;
			bool isLooping = false;
		};

		struct TTransfer {
			SDL_FRect src{ 0,0,0,0 };
			SDL_FRect dest{ 0,0,0,0 };
		};

		class Sprite {
		
		public:
			SDL_Texture* texture = nullptr;//shallow copy is OK, not owner
			SDL_FRect source{ 0,0,0,0 };
			SDL_FRect dest{ 0,0,0,0 };
		
		private:
			static constexpr int frameDelay = 6;
			const std::vector<Reel>* animations = nullptr;//shallow copy is OK, not owner
			const Reel* current = nullptr;//shallow copy is OK, not owner
			int frameIndex = 0;
			int frameTimer = 0;

		public:	
			Sprite() = default;
			Sprite(SDL_Texture* texture, const SDL_FRect& src, const SDL_FRect& dest) :texture(texture), source(src), dest(dest) {}
			Sprite(SDL_Texture* texture, const TTransfer& ttransfer) :texture(texture), source(ttransfer.src), dest(ttransfer.dest) {}

			bool initPlay(const std::vector<Reel>* animationPoints);
			void unInitPlay();
			bool play();
			bool setNewAnimation(const AnimationID id);
			SDL_FRect getAnimatedTextureSource();
			AnimationID getCurrentAnimationID();
		};

		AnimationID animationIDTable(const SMS::State& state);
		void setTTransferField_coordinates(const SDL_FRect& origin, SDL_FRect& dest);
		void setTTransferField_full(const SDL_FRect& origin, SDL_FRect& dest);
	}

}