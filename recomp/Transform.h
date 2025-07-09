#pragma once

#include "point.h"
#include "Rectangle.h"

namespace badEngine {

	class Transform {

		Rectangle rectangle;
		Point velocity;

	public:

		Transform() = default;
		Transform(const Rectangle& rectangle, Point velocity) :rectangle(rectangle), velocity(velocity) {}

		const Rectangle& getRectangle()const
		{
			return rectangle;
		}
		const Point& getVelocity()const
		{
			return velocity;
		}
		
		Rectangle* DataRectangle()
		{
			return &rectangle;
		}
		Point* DataVelocity()
		{
			return &velocity;
		}
		

		void setRectangle(const Rectangle& rectangle)
		{
			this->rectangle = rectangle;
		}
		void setVelocity(const Point& velocity)
		{
			this->velocity = velocity;
		}

		void setPositionManual(const Point& position) {
			rectangle.x = position.x;
			rectangle.y = position.y;
		}
		void setPositionAutomatic() {
			rectangle.x += velocity.x;
			rectangle.y += velocity.y;
		}




	};

}