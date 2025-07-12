#pragma once

/*

Need entities as well

do collisions here

collisions between single components

collisions between lists

resolutions

quadtree optimization

*/

/*
		void resolveAABB_overlapSecond(SDL_FRect& rect, const SDL_Rect& mutator)
		{
			if (mutator.w < mutator.h)
				rect.x += mutator.w;
			else
				rect.y += mutator.h;
		}
		void resolveAABB_overlapFirst(SDL_FRect& rect, const SDL_Rect& mutator)
		{
			if (mutator.w < mutator.h)
				rect.x -= mutator.w;
			else
				rect.y -= mutator.h;
		}
		void resolveOutOfBounds(const SDL_FRect& bounds, SDL_FRect& rect)
		{
			if (rect.x < bounds.x)
				rect.x = bounds.x;
			if (rect.y < bounds.y)
				rect.y = bounds.y;
			if (rect.x + rect.w > bounds.w)
				rect.x = bounds.w - rect.w;
			if (rect.y + rect.h > bounds.h)
				rect.y = bounds.h - rect.h;
		}
*/