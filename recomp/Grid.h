#pragma once

#include <assert.h>
#include "vec2.h"

//need inforamtion save/laod feature using binary files

typedef bool SetTile;

enum class Direction {
	up,
	down,
	left,
	right,
};

class Grid {

	static constexpr int tile_w = 32;
	static constexpr int tile_h = 32;

	bool* is_occupied = nullptr;
	int width = 0;
	int height = 0;

public:

	Grid(int screenWidth, int screenHeight) : width(screenWidth / tile_w), height(screenHeight / tile_h) {
		is_occupied = new bool[width * height] {false};
	}

	bool isFilled(const vec2f& position)const;
	vec2f convertToGrid(const vec2f& position)const;
	vec2f convertToPixel(const vec2f& position)const;
	vec2f getDirection(const Direction direction)const;
	void setTile(const vec2f& position, const SetTile state); //0 being free it, 1 being fill it
	

	~Grid() {
		delete[] is_occupied;
		is_occupied = nullptr;
	}

private:

	Grid(const Grid&) = delete;
	Grid(Grid&&) = delete;
	Grid& operator=(const Grid&) = delete;
};


