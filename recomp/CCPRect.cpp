#include "CCPRect.h"

namespace badEngine {

	//cartesian coordinate physics
	namespace CCP {
		float dotProduct(const SDL_FPoint& point) {
			return (point.x * point.x) + (point.y * point.y);
		}
		float lenght(const SDL_FPoint& point) {
			return SDL_sqrtf(dotProduct(point));
		}
		SDL_FPoint unitVector(const SDL_FPoint& point) {

			float lenght = dotProduct(point);

			lenght = SDL_sqrtf(lenght);

			if (lenght == 0.f) return{ 0.f,0.f };

			return { point.x / lenght, point.y / lenght };
		}
		SDL_FPoint signVector(const SDL_FPoint& point) {
			SDL_FPoint out = { 0.0f, 0.0f };
			if (point.x > 0.0f)
				out.x = 1.0f;
			else if (point.x < 0.0f)
				out.x = -1.0f;
			if (point.y > 0.0f)
				out.y = 1.0f;
			else if (point.y < 0.0f)
				out.y = -1.0f;
			return out;
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

			if (overlapX > 0.0f && overlapY > 0.0f) {
				output->x = dx;
				output->y = dy;
				output->w = overlapX;
				output->h = overlapY;
				return true;
			}
			return false;
		}
		bool collisionRayProjection(const SDL_FPoint& rayOrigin, const SDL_FPoint& rayVector, const SDL_FRect& target, float& hitTime, SDL_FPoint* cp, SDL_FPoint* cn) {
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
			if (tNear.x > tFar.x) swapFloat(tNear.x, tFar.x);
			if (tNear.y > tFar.y) swapFloat(tNear.y, tFar.y);

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
					if (rayVector.x < 0.0f)
						*cn = { 1.0f, 0.0f };
					else
						*cn = { -1.0f, 0.0f };
				else if (tNear.x < tNear.y)
					if (rayVector.y < 0.0f)
						*cn = { 0.0f, 1.0f };
					else
						*cn = { 0.0f, -1.0f };
			}

			return true;
		}
		bool collisionEnhancedRayProjection(const HitBox& first, const HitBox& second, float& hitTime, SDL_FPoint* cp, SDL_FPoint* cn) {
			// Relative motion
			SDL_FPoint relVelocity = {
				first.velocity.x - second.velocity.x,
				first.velocity.y - second.velocity.y
			};
			// Early out if no motion
			if (relVelocity.x == 0.0f && relVelocity.y == 0.0f) return false;

			//adjust target rectangle since ray origin is from the center not top left
			SDL_FRect expandedTarget = {
				second.rectangle.x - first.halfWidth,
				second.rectangle.y - first.halfHeight,
				second.rectangle.w + first.rectangle.w,
				second.rectangle.h + first.rectangle.h
			};

			// Ray origin is the center of this object
			SDL_FPoint rayOrigin = {
				first.rectangle.x + first.halfWidth,
				first.rectangle.y + first.halfHeight
			};

			if (collisionRayProjection(rayOrigin, relVelocity, expandedTarget, hitTime, cp, cn)) {
				return (hitTime >= 0.0f && hitTime < 1.0f);
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
		void resolveOutOfBounds(const SDL_FRect& bounds, SDL_FRect& rect) {
			if (rect.x < bounds.x) {
				rect.x = bounds.x;
			}
			if (rect.y < bounds.y) {
				rect.y = bounds.y;
			}
			if (rect.x + rect.w > bounds.w) {
				rect.x = bounds.w - rect.w;
			}
			if (rect.y + rect.h > bounds.h) {
				rect.y = bounds.h - rect.h;
			}
		}


	}
}