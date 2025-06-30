#pragma once

namespace badEngine {

	class Point {

	public:
		float x = 0.0f;
		float y = 0.0f;

		Point() = default;
		Point(const float x, const float y) :x(x), y(y) {}

		Point operator+(const Point& p)const;
		Point& operator+=(const Point& p);
		Point operator-(const Point& p)const;
		Point& operator-=(const Point& p);
		Point operator*(const float scalar)const;
		Point& operator*=(const float scalar);
		Point operator/(const float scalar)const;
		Point& operator/=(const float scalar);
		bool operator==(const Point& p)const;
		bool operator!=(const Point& p)const;

		float dot()const;
		static float dot(const Point& p);
		float lenght()const;
		static float lenght(const Point& p);
		Point unitVector()const;
		static Point unitVector(const Point& p);
		Point signVector()const;
		static Point signVector(const Point& p);
	};
}

