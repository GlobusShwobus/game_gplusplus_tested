#pragma once

class vec2f {
public:
	float x = 0;
	float y = 0;

	vec2f(const float X, const float Y) :x(X), y(Y) {}

	//equate
	bool operator==(const vec2f& other)const {
		return(x == other.x) && (y == other.y);
	}
	bool operator!=(const vec2f& other)const {
		return(x != other.x) && (y != other.y);
	}
	//addition
	vec2f operator+(const vec2f& rhs)const {
		return { x + rhs.x, y + rhs.y };
	}
	vec2f& operator+=(const vec2f& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	//subtraction
	vec2f operator-(const vec2f& rhs)const {
		return { x - rhs.x, y - rhs.y };
	}
	vec2f operator-=(const vec2f& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	//multiplication
	vec2f operator*(const float scale)const {
		return { x * scale, y * scale };
	}
	vec2f& operator*=(const float scale) {
		x *= scale;
		y *= scale;
		return *this;
	}
	//division
	vec2f operator/(const int scale)const {
		return { x / scale, y / scale };
	}
	vec2f& operator/=(const int scale) {
		x /= scale;
		y /= scale;
		return *this;
	}
};