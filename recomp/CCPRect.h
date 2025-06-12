#pragma once

#include "SDL3/SDL.h"

namespace badEngine {

	//cartesian coordinate physics
	namespace CCP {

		struct HitBox {
			SDL_FRect rectangle{ 0.f,0.f,0.f,0.f };
			SDL_FPoint velocity{ 0.f,0.f };

			float halfWidth = 0.f;
			float halfHeight = 0.f;

			HitBox() = default;
			HitBox(float x, float y, float w, float h) :rectangle{ x,y,w,h }, halfWidth(w * 0.5f), halfHeight(h * 0.5f) {}
			HitBox(const SDL_FRect& rectangle, const SDL_FPoint& vel) :rectangle(rectangle), velocity(vel), halfWidth(rectangle.w * 0.5f), halfHeight(rectangle.h * 0.5f) {}
		};

		float dotProduct(const SDL_FPoint& point);
		float lenght(const SDL_FPoint& point);
		SDL_FPoint unitVector(const SDL_FPoint& point);
		SDL_FPoint signVector(const SDL_FPoint& point);

		bool containsPoint(const SDL_FRect& rect, const SDL_FPoint& point);
		bool containsRect(const SDL_FRect& outer, const SDL_FRect& inner);
		bool collisionAABB(const SDL_FRect& a, const SDL_FRect& b);
		bool collisionEnhancedAABB(const HitBox& first, const HitBox& second, SDL_Rect* output);
		bool collisionRayProjection(const SDL_FPoint& rayOrigin, const SDL_FPoint& rayVector, const SDL_FRect& target, float& hitTime, SDL_FPoint* cp = nullptr, SDL_FPoint* cn = nullptr);
		bool collisionEnhancedRayProjection(const HitBox& first, const HitBox& second, float& hitTime, SDL_FPoint* cp = nullptr, SDL_FPoint* cn = nullptr);

		void resolveAABB_overlapSecond(SDL_FRect& rect, const SDL_Rect& mutator);
		void resolveAABB_overlapFirst(SDL_FRect& rect, const SDL_Rect& mutator);
		void resolveOutOfBounds(const SDL_FRect& bounds, SDL_FRect& rect);
		


		inline void swapFloat(float& a, float& b) {
			float temp = a;
			a = b;
			b = temp;
		}
	}

}