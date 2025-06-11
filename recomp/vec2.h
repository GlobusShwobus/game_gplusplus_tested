#pragma once

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
		
		SDL_FPoint operator+(const SDL_FPoint& a, const SDL_FPoint& b) {
			return { a.x + b.x, a.y + b.y };
		}
		SDL_FPoint& operator+=(SDL_FPoint& a, const SDL_FPoint& b) {
			a.x += b.x;
			a.y += b.y;
			return a;
		}
		SDL_FPoint operator-(const SDL_FPoint& a, const SDL_FPoint& b) {
			return { a.x - b.x, a.y - b.y };
		}
		SDL_FPoint& operator-=(SDL_FPoint& a, const SDL_FPoint& b) {
			a.x -= b.x;
			a.y -= b.y;
			return a;
		}
		SDL_FPoint operator*(const SDL_FPoint& a, float scalar) {
			return { a.x * scalar, a.y * scalar };
		}
		SDL_FPoint& operator*=(SDL_FPoint& a, float scalar) {
			a.x *= scalar;
			a.y *= scalar;
			return a;
		}
		SDL_FPoint operator/(const SDL_FPoint& a, float scalar) {
			return { a.x / scalar, a.y / scalar };
		}
		SDL_FPoint& operator/=(SDL_FPoint& a, float scalar) {
			a.x /= scalar;
			a.y /= scalar;
			return a;
		}
		float dotProduct(const SDL_FPoint& point) {
			return (point.x * point.x) + (point.y * point.y);
		}
		float lenght(const SDL_FPoint& point) {
			return SDL_sqrtf(dotProduct(point));
		}
		SDL_FPoint unitVector(const SDL_FPoint& point) {

			float lenght = dotProduct(point);

			lenght = SDL_sqrtf(lenght);

			if (lenght == 0.0f) return{ 0.0f,0.0f };

			return point / lenght;
		}

		bool containsPoint(const SDL_FRect& rect, const SDL_FPoint& point) {
			return (
				point.x >= rect.x &&
				point.y >= rect.y &&
				point.x < rect.x + rect.w &&
				point.y < rect.y + rect.h);
		}
		bool containsRect(const SDL_FRect& outer, const SDL_FRect& inner) {
			return (
				inner.x >= outer.x &&
				inner.y >= outer.y &&
				inner.x + inner.w <= outer.x + outer.w &&
				inner.y + inner.h <= outer.y + outer.h);
		}
		bool collisionAABB(const SDL_FRect& a, const SDL_FRect& b) {
			return (
				a.x < b.x + b.w &&
				a.x + a.w > b.x &&
				a.y < b.y + b.h &&
				a.y + a.h > b.y
				);
		}
		bool collisionEnhancedAABB(const HitBox& first, const HitBox& second, SDL_Rect* output) {
			if (!output) return false;

			float dx = (first.rectangle.x + first.halfWidth) - (second.rectangle.x + second.halfWidth);
			float dy = (first.rectangle.y + first.halfHeight) - (second.rectangle.y + second.halfHeight);

			float overlapX = (first.halfWidth + second.halfWidth) - SDL_fabs(dx);
			float overlapY = (first.halfHeight + second.halfHeight) - SDL_fabs(dy);

			if (overlapX > 0 && overlapY > 0) {
				output->x = dx;
				output->y = dy;
				output->w = overlapX;
				output->h = overlapY;
				return true;
			}
			return false;
		}

		bool collisionRayProjection(const SDL_FPoint& rayOrigin, const SDL_FPoint& rayVector, const SDL_FRect& target, float& hitTime, SDL_FPoint* cp = nullptr, SDL_FPoint* cn = nullptr) {
			// Inverse direction, idfk why, but it is OK to divide floats by 0, creates an infiniti and later creates a NaN
			SDL_FPoint inverse = {
				 1.0f / rayVector.x,
				  1.0f / rayVector.y
			};
			// Calculate intersections with rectangle bounding axes
			SDL_FPoint tNear = {
				(target.x - rayOrigin.x) * inverse.x,
				(target.y - rayOrigin.y) * inverse.y
			};
			SDL_FPoint tFar = {
				(target.x + target.w - rayOrigin.x) * inverse.x,
				(target.y + target.h - rayOrigin.y) * inverse.y
			};
			//check for bullshit floating value
			if (SDL_isnan(tNear.x) || SDL_isnan(tNear.y) || SDL_isnan(tFar.x) || SDL_isnan(tFar.y)) return false;

			// Swap near and far
			if (tNear.x > tFar.x) std::swap(tNear.x, tFar.x);
			if (tNear.y > tFar.y) std::swap(tNear.y, tFar.y);

			// if no hit
			if (tNear.x > tFar.y || tNear.y > tFar.x) return false;

			hitTime = SDL_max(tNear.x, tNear.y);
			float tHitFar = SDL_min(tFar.x, tFar.y);

			//if hit but opposite direction (infinite plane)
			if (tHitFar < 0.0f) return false;

			//set the point where contact was made, idk what to do with it, can remove later tho
			if (cp) {
				*cp = {
					(rayVector.x * tHitFar) + rayOrigin.x,
					(rayVector.y * tHitFar) + rayOrigin.y
				};
			}
			//get normalized Sign value
			if (cn) {
				if (tNear.x > tNear.y)
					if (rayVector.x < 0)
						*cn = { 1, 0 };
					else
						*cn = { -1, 0 };
				else if (tNear.x < tNear.y)
					if (rayVector.y < 0)
						*cn = { 0, 1 };
					else
						*cn = { 0, -1 };
			}

			return true;
		}
		void resolveAABB_overlapSecond(SDL_FRect& rect, const SDL_Rect& mutator) {
			if (mutator.w < mutator.h) {
				rect.x += mutator.w;
			}
			else {
				rect.y += mutator.h;
			}
		}
		void resolveAABB_overlapFirst(SDL_FRect& rect, const SDL_Rect& mutator) {
			if (mutator.w < mutator.h) {
				rect.x -= mutator.w;
			}
			else {
				rect.y -= mutator.h;
			}
		}

		



	}

}