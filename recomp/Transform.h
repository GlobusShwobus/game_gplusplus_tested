#pragma once

#include "Vek2.h"
#include "Rectangle.h"

#define BADENGINE_MAX(x,y) (((x) > (y)) ? (x) : (y))
#define BADENGINE_MIN(x,y) (((x) < (y)) ? (x) : (y))

namespace badEngine {

	class Transform {

		Rectangle rectangle;
		Vek2 velocity;

	public:

		Transform() = default;
		Transform(const Rectangle& rectangle, Vek2 velocity) :rectangle(rectangle), velocity(velocity) {}

		inline const Rectangle& getRectangle()const
		{
			return rectangle;
		}
		inline const Vek2& getVelocity()const
		{
			return velocity;
		}

		inline Rectangle* DataRectangle()
		{
			return &rectangle;
		}
		inline Vek2* DataVelocity()
		{
			return &velocity;
		}

		inline void setRectangle(float x, float y, float w, float h)
		{
			rectangle = { x,y,w,h };
		}
		inline void setRectangle(const Rectangle& rectangle)
		{
			this->rectangle = rectangle;
		}
		inline void setVelocity(float x, float y)
		{
			velocity = { x,y };
		}
		inline void setVelocity(const Vek2& velocity)
		{
			this->velocity = velocity;
		}

		inline void setPositionManual(float x, float y)
		{
			rectangle.x = x;
			rectangle.y = y;
		}
		inline void setPositionManual(const Vek2& position)
		{
			rectangle.x = position.x;
			rectangle.y = position.y;
		}
		inline void setPositionAutomatic()
		{
			rectangle.x += velocity.x;
			rectangle.y += velocity.y;
		}

		static bool rayCollision(const Vek2& rayOrigin, const Vek2& rayVector, const Rectangle& target, float& hitTime, Vek2* contantPoint = nullptr, Vek2* contactNormal = nullptr);
		bool rayCollisionEnhanced(const Rectangle& bRect, const Vek2& bVel, float& hitTime, Vek2* contactPoint = nullptr, Vek2* contactNormal = nullptr)const;
		bool rayCollisionEnhanced(const Transform& target, float& hitTime, Vek2* contactPoint = nullptr, Vek2* contactNormal = nullptr)const;
		static bool rayCollisionEnhanced(const Transform& a, const Transform& b, float& hitTime, Vek2* contactPoint = nullptr, Vek2* contactNormal = nullptr);
		static bool rayCollisionEnhanced(const Rectangle& aRect, const Vek2& aVel, const Rectangle& bRect, const Vek2& bVel, float& hitTime, Vek2* contactPoint = nullptr, Vek2* contactNormal = nullptr);

	private:

		static inline void swap(float& a, float& b)
		{
			float temp = a;
			a = b;
			b = temp;
		}

	};
}