#pragma once

#define PI 3.14159265
#define NOMINMAX

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include <Windows.h>
#include <exception>
#include <random>
#include <cmath>
#include "json.hpp"
#include <algorithm>





//       EVERYTHING TEMPORARYLY OR PERMANENTLY DEPRICATED  





/*
namespace line {

	struct line {
		vec2f beg = { 0,0 };
		vec2f end = { 0,0 };

		line(const vec2f& s, const vec2f& e) :beg(s), end(e) {}
	};

	float Slope(const vec2f& first, const vec2f& second);
	float Angle(const line& first, const line& second);
	vec2f Intersection(const line& line1, const line& line2);
	float Sqrt(const float num);
}


enum class CollisionSide {
	left, top, right, bottom, no_overlap
};

class BoundingBox {
public:
	int x = 0.f;
	int y = 0.f;
	int width = 0.f;
	int height = 0.f;

	BoundingBox(const vec2i& pos, const vec2i& dimensions) :x(pos.x), y(pos.y), width(dimensions.x), height(dimensions.y) {}
	BoundingBox(int X, int Y, int WIDTH, int HEIGHT) :x(X), y(Y), width(WIDTH), height(HEIGHT){}
	BoundingBox() = default;

	BoundingBox& GetBoundingBox();
	bool Intersects(const BoundingBox& another)const;
	CollisionSide GetCollisionSide(const BoundingBox& another)const;
};

namespace PoSMath {

	vec2f ReflectVelocity(const BoundingBox& reflected, const BoundingBox& from, vec2i& vel);
	vec2f BlockFurtherMove(const BoundingBox& guilty, const BoundingBox& another);



	/*
	float Dist(const vec2i& end);
	float Lenght();
	vec2i Normalized();
	*/

	/*

	class CInput {
	public:
		bool up = false;
		bool left = false;
		bool right = false;
		bool down = false;
		bool shield = false;
		bool shoot = false;

		CInput() = default;
	};

	*/