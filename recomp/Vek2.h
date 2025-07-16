#pragma once
#include <cmath>
namespace badEngine {

	class Vek2 {

	public:
		float x = 0.0f;
		float y = 0.0f;

		Vek2() = default;
		Vek2(float x, float y) :x(x), y(y) {}

		inline Vek2 operator+(const Vek2& p)const
		{
			return{ x + p.x, y + p.y };
		}
		inline Vek2& operator+=(const Vek2& p)
		{
			x += p.x; y += p.y;
			return *this;
		}
		inline Vek2 operator-(const Vek2& p)const
		{
			return{ x - p.x, y - p.y };
		}
		inline Vek2& operator-=(const Vek2& p)
		{
			x -= p.x; y -= p.y;
			return *this;
		}
		inline Vek2 operator*(const float scalar)const
		{
			return{ x * scalar, y * scalar };
		}
		inline Vek2& operator*=(const float scalar)
		{
			x *= scalar; y *= scalar;
			return *this;
		}
		inline Vek2 operator/(const float scalar)const
		{
			return{ x / scalar, y / scalar };
		}
		inline Vek2& operator/=(const float scalar)
		{
			x /= scalar; y /= scalar;
			return *this;
		}
		inline bool operator==(const Vek2& p)const
		{
			return x == p.x && y == p.y;
		}
		inline bool operator!=(const Vek2& p)const
		{
			return x != p.x || y != p.y;
		}

		inline float dot()const
		{
			return Vek2::dot(*this);
		}
		static inline float dot(const Vek2& p)
		{
			return(p.x * p.x) + (p.y * p.y);
		}

		inline float lenght()const
		{
			return Vek2::lenght(*this);
		}
		static inline float lenght(const Vek2& p)
		{
			return std::sqrtf(dot(p));
		}

		inline Vek2 inverse()const
		{
			return Vek2::inverse(*this);
		}
		static inline Vek2 inverse(const Vek2& p) {
			return{ 1.0f / p.x, 1.0f / p.y };
		}

		inline Vek2 unitVector()const
		{
			return Vek2::unitVector(*this);
		}
		static inline Vek2 unitVector(const Vek2& p)
		{
			float len = Vek2::lenght(p);
			if (len == 0.0f)
				return{ 0.0f,0.0f };
			else
				return { p.x / len, p.y / len };
		}

		inline Vek2 signVector()const
		{
			return Vek2::signVector(*this);
		}
		static inline Vek2 signVector(const Vek2& p)
		{
			Vek2 z{ 0.0f,0.0f };
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

