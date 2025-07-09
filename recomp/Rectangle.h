#pragma once

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
	};

}