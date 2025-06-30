#include "point.h"
#include <cmath>

namespace badEngine {

	Point Point::operator+(const Point& p)const
	{
		return{ x + p.x, y + p.y };
	}
	Point& Point::operator+=(const Point& p)
	{
		x += p.x; y += p.y;
		return *this;
	}
	Point Point::operator-(const Point& p)const
	{
		return{ x - p.x, y - p.y };
	}
	Point& Point::operator-=(const Point& p)
	{
		x -= p.x; y -= p.y;
		return *this;
	}
	Point Point::operator*(const float scalar)const
	{
		return{ x * scalar, y * scalar };
	}
	Point& Point::operator*=(const float scalar)
	{
		x *= scalar; y *= scalar;
		return *this;
	}
	Point Point::operator/(const float scalar)const
	{
		return{ x / scalar, y / scalar };
	}
	Point& Point::operator/=(const float scalar)
	{
		x /= scalar; y /= scalar;
		return *this;
	}
	bool Point::operator==(const Point& p)const
	{
		return x == p.x && y == p.y;
	}
	bool Point::operator!=(const Point& p)const
	{
		return x != p.x || y != p.y;
	}

	float Point::dot()const
	{
		return (x * x) + (y * y);
	}
	float Point::dot(const Point& p)
	{
		return(p.x * p.x) + (p.y * p.y);
	}
	float Point::lenght()const
	{
		return std::sqrtf(dot());
	}
	float Point::lenght(const Point& p)
	{
		return std::sqrtf(Point::dot(p));
	}
	Point Point::unitVector()const
	{
		float len = lenght();
		if (len == 0.0f)
			return{ 0.0f,0.0f };
		else
			return { x / len,y / len };
	}
	Point Point::unitVector(const Point& p)
	{
		float len = Point::lenght(p);
		if (len == 0.0f)
			return{ 0.0f,0.0f };
		else
			return { p.x / len, p.y / len };
	}
	Point Point::signVector()const
	{
		Point p{ 0.0f,0.0f };
		if (x > 0.0f)
			p.x = 1.0f;
		else if (x < 0.0f)
			p.x = -1.0f;
		if (y > 0.0f)
			p.y = 1.0f;
		else if (y < 0.0f)
			p.y = -1.0f;
		return p;
	}
	Point Point::signVector(const Point& p)
	{
		Point z{ 0.0f,0.0f };
		if (p.x > 0.0f)
			z.x = 1.0f;
		else if (p.x < 0.0f)
			z.x = -1.0f;
		if (p.y > 0.0f)
			z.y = 1.0f;
		else if (p.y < 0.0f)
			z.y = -1.0f;
		return z;
	}
}