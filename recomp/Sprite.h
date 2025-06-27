#pragma once

#include <vector>
#include <memory>
#include "HashKeys.h"
#include "SDL3/SDL.h"

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
			std::shared_ptr<SDL_Texture> texture;
			SDL_FRect source{ 0,0,0,0 };
			SDL_FRect dest{ 0,0,0,0 };
		
		private:
			static constexpr int frameDelay = 6;
			std::shared_ptr<const std::vector<Reel>> animations;
			const Reel* current = nullptr;//points to the current animation
			int frameIndex = 0;
			int frameTimer = 0;

		public:	
			Sprite() = default;
			Sprite(std::shared_ptr<SDL_Texture> texture, const SDL_FRect& src, const SDL_FRect& dest) :texture(texture), source(src), dest(dest) {}
			Sprite(std::shared_ptr<SDL_Texture> texture, const TTransfer& ttransfer) :texture(texture), source(ttransfer.src), dest(ttransfer.dest) {}

			bool initPlay(std::shared_ptr<const std::vector<Reel>> animationPoints);
			void unInitPlay();
			bool play();
			bool setNewAnimation(const AnimationID id);
			SDL_FRect getAnimatedTextureSource();
			AnimationID getCurrentAnimationID();
		};

		void setTTransferField_coordinates(const SDL_FRect& origin, SDL_FRect& dest);
		void setTTransferField_full(const SDL_FRect& origin, SDL_FRect& dest);
	}

}