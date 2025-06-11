#pragma once
#include <cmath>
#include "SDL3/SDL.h"
namespace badEngine {

	//cartesian coordinate physics
	namespace CCP {

		struct HitBox {
			SDL_FRect rectangle{ 0,0,0,0 };
			SDL_FPoint velocity{ 0,0 };

			float halfWidth = 0.f;
			float halfHeight = 0.f;

			HitBox() = default;
			HitBox(float x, float y, float w, float h) :rectangle{ x,y,w,h }, halfWidth(w * 0.5f), halfHeight(h * 0.5f) {}
			HitBox(const SDL_FRect& rectangle, const SDL_FPoint& vel) :rectangle(rectangle), velocity(vel), halfWidth(rectangle.w * 0.5f), halfHeight(rectangle.h * 0.5f) {}
		};


		SDL_FRect addXY(const SDL_FRect& rect, const SDL_FPoint& point);
		SDL_FPoint operator+(const SDL_FPoint& a, const SDL_FPoint& b);
		SDL_FPoint& operator+=(SDL_FPoint& a, const SDL_FPoint& b);
		SDL_FPoint operator-(const SDL_FPoint& a, const SDL_FPoint& b);
		SDL_FPoint& operator-=(SDL_FPoint& a, const SDL_FPoint& b);
		SDL_FPoint operator*(const SDL_FPoint& a, float scalar);
		SDL_FPoint& operator*=(SDL_FPoint& a, float scalar);
		SDL_FPoint operator/(const SDL_FPoint& a, float scalar);
		SDL_FPoint& operator/=(SDL_FPoint& a, float scalar);
		float dotProduct(const SDL_FPoint& point);
		float lenght(const SDL_FPoint& point);
		SDL_FPoint unitVector(const SDL_FPoint& point);

		bool containsPoint(const SDL_FRect& rect, const SDL_FPoint& point);
		bool containsRect(const SDL_FRect& outer, const SDL_FRect& inner);
		bool collisionAABB(const SDL_FRect& a, const SDL_FRect& b);
		bool collisionEnhancedAABB(const HitBox& first, const HitBox& second, SDL_Rect* output);
		void resolveAABB_overlapFirst(SDL_FRect& rect, const SDL_Rect& mutator);
		void resolveAABB_overlapSecond(SDL_FRect& rect, const SDL_Rect& mutator);

	}

}