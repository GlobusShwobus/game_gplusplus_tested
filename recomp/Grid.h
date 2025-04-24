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

class InstancedGrid {

	static constexpr int tile_w = 32;
	static constexpr int tile_h = 32;

	bool* is_occupied = nullptr;
	int width = 0;
	int height = 0;

public:

	InstancedGrid(int screenWidth, int screenHeight) : width(screenWidth / tile_w), height(screenHeight / tile_h) {
		is_occupied = new bool[width * height] {false};
	}

	bool isFilled(const vec2i& position);
	vec2i getNext(const vec2i& currentPos, const Direction direction);
	void setTile(const vec2i& position, const SetTile state); //0 being free it, 1 being fill it

	~InstancedGrid() {
		delete[] is_occupied;
		is_occupied = nullptr;
	}

private:
	vec2i convertToGrid(const vec2i& position);
	vec2i convertToPixel(const vec2i& position);

	vec2i getMoveDirection(const Direction direction);


	InstancedGrid(const InstancedGrid&) = delete;
	InstancedGrid(InstancedGrid&&) = delete;
	InstancedGrid& operator=(const InstancedGrid&) = delete;
};


