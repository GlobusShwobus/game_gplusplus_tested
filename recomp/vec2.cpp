#include "vec2.h"

namespace badEngine {

	//cartesian coordinate physics
	namespace CCP {

		SDL_FRect addXY(const SDL_FRect& rect, const SDL_FPoint& point) {
			return{
				rect.x + point.x,
				rect.y + point.y,
				rect.w, rect.h };
		}
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
			return std::sqrtf(dotProduct(point));
		}
		SDL_FPoint unitVector(const SDL_FPoint& point) {

			float lenght = dotProduct(point);

			lenght = std::sqrtf(lenght);

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