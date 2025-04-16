#pragma once
template <typename T> class vec2 {
public:
	T x = 0;
	T y = 0;

	vec2(const T X, const T Y) :x(X), y(Y) {}

	//equate
	bool operator==(const vec2& other)const {
		return(x == other.x) && (y == other.y);
	}
	bool operator!=(const vec2& other)const {
		return(x != other.x) && (y != other.y);
	}
	//addition
	vec2 operator+(const vec2& rhs)const {
		return vec2(x + rhs.x, y + rhs.y);
	}
	vec2& operator+=(const vec2& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	//subtraction
	vec2 operator-(const vec2& rhs)const {
		return vec2f(x - rhs.x, y - rhs.y);
	}
	vec2 operator-=(const vec2& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	//multiplication
	vec2 operator*(const float scale)const {
		return vec2(x * scale, y * scale);
	}
	vec2& operator*=(const float scale) {
		x *= scale;
		y *= scale;
		return *this;
	}
	//division
	vec2 operator/(const float scale)const {
		return vec2(x / scale, y / scale);
	}
	vec2& operator/=(const float scale) {
		x /= scale;
		y /= scale;
		return *this;
	}
};

typedef vec2<int> vec2i;
typedef vec2<float> vec2f;