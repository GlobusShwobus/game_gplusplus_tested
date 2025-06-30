#pragma once

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
			return (x * x) + (y * y);
		}
		static inline float dot(const Point& p)
		{
			return(p.x * p.x) + (p.y * p.y);
		}
		inline float lenght()const
		{
			return std::sqrtf(dot());
		}
		static inline float lenght(const Point& p)
		{
			return std::sqrtf(Point::dot(p));
		}
		inline Point unitVector()const
		{
			float len = lenght();
			if (len == 0.0f)
				return{ 0.0f,0.0f };
			else
				return { x / len,y / len };
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

