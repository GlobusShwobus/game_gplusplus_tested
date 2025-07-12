#pragma once

#include "point.h"
#include "Rectangle.h"

#define BADENGINE_MAX(x,y) (((x) > (y)) ? (x) : (y))
#define BADENGINE_MIN(x,y) (((x) < (y)) ? (x) : (y))

namespace badEngine {

	class Transform {

		Rectangle rectangle;
		Point velocity;

	public:

		Transform() = default;
		Transform(const Rectangle& rectangle, Point velocity) :rectangle(rectangle), velocity(velocity) {}

		inline const Rectangle& getRectangle()const
		{
			return rectangle;
		}
		inline const Point& getVelocity()const
		{
			return velocity;
		}

		inline Rectangle* DataRectangle()
		{
			return &rectangle;
		}
		inline Point* DataVelocity()
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
		inline void setVelocity(const Point& velocity)
		{
			this->velocity = velocity;
		}

		inline void setPositionManual(float x, float y)
		{
			rectangle.x = x;
			rectangle.y = y;
		}
		inline void setPositionManual(const Point& position)
		{
			rectangle.x = position.x;
			rectangle.y = position.y;
		}
		inline void setPositionAutomatic()
		{
			rectangle.x += velocity.x;
			rectangle.y += velocity.y;
		}

		static bool rayCollision(const Point& rayOrigin, const Point& rayVector, const Rectangle& target, float& hitTime, Point* contantPoint = nullptr, Point* contactNormal = nullptr);
		bool rayCollisionEnhanced(const Rectangle& bRect, const Point& bVel, float& hitTime, Point* contactPoint = nullptr, Point* contactNormal = nullptr)const;
		bool rayCollisionEnhanced(const Transform& target, float& hitTime, Point* contactPoint = nullptr, Point* contactNormal = nullptr)const;
		static bool rayCollisionEnhanced(const Transform& a, const Transform& b, float& hitTime, Point* contactPoint = nullptr, Point* contactNormal = nullptr);
		static bool rayCollisionEnhanced(const Rectangle& aRect, const Point& aVel, const Rectangle& bRect, const Point& bVel, float& hitTime, Point* contactPoint = nullptr, Point* contactNormal = nullptr);

	private:

		static inline void swap(float& a, float& b)
		{
			float temp = a;
			a = b;
			b = temp;
		}

	};
}