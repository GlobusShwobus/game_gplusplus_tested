#pragma once
#include <cmath>

namespace badEngine {

	class Rectangle {
	public:
		float x = 0.0f;
		float y = 0.0f;
		float w = 0.0f;
		float h = 0.0f;

		Rectangle() = default;
		Rectangle(float x, float y, float w, float h) :x(x), y(y), w(w), h(h) {}

		inline bool containsPoint(float cordX, float cordY)const 
		{
			return Rectangle::containsPoint(*this, cordX, cordY);
		}
		static inline bool containsPoint(const Rectangle& rect, float cordX, float cordY)
		{
			return (
				cordX >= rect.x &&
				cordY >= rect.y &&
				cordX < rect.x + rect.w &&
				cordY < rect.y + rect.h);
		}

		inline bool containsRect(const Rectangle& other)const
		{
			return Rectangle::containsRect(*this, other);
		}
		static inline bool containsRect(const Rectangle& larger, const Rectangle& smaller)
		{
			return (
				smaller.x >= larger.x &&
				smaller.y >= larger.y &&
				smaller.x + smaller.w <= larger.x + larger.w &&
				smaller.y + smaller.h <= larger.y + larger.h);
		}

		inline bool intersects(const Rectangle& other)const
		{
			return Rectangle::intersects(*this, other);
		}
		static inline bool intersects(const Rectangle& a, const Rectangle& b)
		{
			return (
				a.x < b.x + b.w &&
				a.x + a.w > b.x &&
				a.y < b.y + b.h &&
				a.y + a.h > b.y
				);
		}
		bool intersectsEnhanced(const Rectangle& other, Rectangle* output)const {
			return Rectangle::intersectsEnhanced(*this, other, output);
		}
		static bool intersectsEnhanced(const Rectangle& a, const Rectangle& b, Rectangle* output) {

			float dx = center(a.x, a.w) - center(b.x, b.w);
			float dy = center(a.y, a.h) - center(b.y, b.h);

			float overlapX = overlap(a.w, b.w, dx);
			float overlapY = overlap(a.h, b.h, dy);

			if (overlapX < 0.0f && overlapY < 0.0f)
				return false;

			if (output)
				*output = { dx,dy,overlapX, overlapY };
		
			return true;
		}

		inline float centerX()const {
			return Rectangle::center(x, w);
		}
		inline float centerY()const {
			return Rectangle::center(y, h);
		}
		inline float halfWidth()const {
			return Rectangle::half(w);
		}
		inline float halfHeight()const {
			return Rectangle::half(h);
		}

	private:

		static inline float half(float lenght) {
			return lenght * 0.5f;
		}
		static inline float center(float pos, float lenght) {
			return pos + Rectangle::half(lenght);
		}
		static inline float overlap(float dim1, float dim2, float dx) {
			return Rectangle::half(dim1) + Rectangle::half(dim2) - std::fabs(dx);
		}

	};

}