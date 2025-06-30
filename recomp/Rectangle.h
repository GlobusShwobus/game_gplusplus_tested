#pragma once

namespace badEngine {

	class RawRectangle {
	public:
		float x = 0.0f;
		float y = 0.0f;
		float w = 0.0f;
		float h = 0.0f;

		RawRectangle() = default;
		RawRectangle(float x, float y, float w, float h) :x(x), y(y), w(w), h(h) {}

		inline bool containsPoint(float cordX, float cordY)const 
		{
			return RawRectangle::containsPoint(*this, cordX, cordY);
		}
		static inline bool containsPoint(const RawRectangle& rect, float cordX, float cordY)
		{
			return (
				cordX >= rect.x &&
				cordY >= rect.y &&
				cordX < rect.x + rect.w &&
				cordY < rect.y + rect.h);
		}
		inline bool containsRect(const RawRectangle& other)const
		{
			return RawRectangle::containsRect(*this, other);
		}
		static inline bool containsRect(const RawRectangle& larger, const RawRectangle& smaller)
		{
			return (
				smaller.x >= larger.x &&
				smaller.y >= larger.y &&
				smaller.x + smaller.w <= larger.x + larger.w &&
				smaller.y + smaller.h <= larger.y + larger.h);
		}
	};

}