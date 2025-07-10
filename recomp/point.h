#pragma once
#include <cmath>
namespace badEngine {

	class Point {

	public:
		float x = 0.0f;
		float y = 0.0f;

		Point() = default;
		Point(const float x, const float y) :x(x), y(y) {}

		inline Point operator+(const Point& p)const
		{
			return{ x + p.x, y + p.y };
		}
		inline Point& operator+=(const Point& p)
		{
			x += p.x; y += p.y;
			return *this;
		}
		inline Point operator-(const Point& p)const
		{
			return{ x - p.x, y - p.y };
		}
		inline Point& operator-=(const Point& p)
		{
			x -= p.x; y -= p.y;
			return *this;
		}
		inline Point operator*(const float scalar)const
		{
			return{ x * scalar, y * scalar };
		}
		inline Point& operator*=(const float scalar)
		{
			x *= scalar; y *= scalar;
			return *this;
		}
		inline Point operator/(const float scalar)const
		{
			return{ x / scalar, y / scalar };
		}
		inline Point& operator/=(const float scalar)
		{
			x /= scalar; y /= scalar;
			return *this;
		}
		inline bool operator==(const Point& p)const
		{
			return x == p.x && y == p.y;
		}
		inline bool operator!=(const Point& p)const
		{
			return x != p.x || y != p.y;
		}

		inline float dot()const
		{
			return Point::dot(*this);
		}
		static inline float dot(const Point& p)
		{
			return(p.x * p.x) + (p.y * p.y);
		}

		inline float lenght()const
		{
			return Point::lenght(*this);
		}
		static inline float lenght(const Point& p)
		{
			return std::sqrtf(dot(p));
		}

		inline Point inverse()const
		{
			return Point::inverse(*this);
		}
		static inline Point inverse(const Point& p) {
			return{ 1.0f / p.x, 1.0f / p.y };
		}

		inline Point unitVector()const
		{
			return Point::unitVector(*this);
		}
		static inline Point unitVector(const Point& p)
		{
			float len = Point::lenght(p);
			if (len == 0.0f)
				return{ 0.0f,0.0f };
			else
				return { p.x / len, p.y / len };
		}

		inline Point signVector()const
		{
			return Point::signVector(*this);
		}
		static inline Point signVector(const Point& p)
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
	};
}

