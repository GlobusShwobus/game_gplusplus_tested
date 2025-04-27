#pragma once

#include <assert.h>
#include "vec2.h"

typedef bool SetTile;

enum class Direction {	//yeet this function in misc pile
	up,
	down,
	left,
	right,
};

/*

NOTE TO SELF!!!

There is a missmatch between tile positions and real pixel positions, how SDL_FRect is floats so it's simple to just work with floats
This means there is a truncation issue. Currently it works as intended by truncating floats to integers and accessing correct tiles since floats trunc in one direction

But there is always an invisible asshole (potential) bug to keep in mind

*/


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
	void setTile(const vec2f& position, const SetTile state); //0 being free it, 1 being fill it
	
	~Grid() {
		delete[] is_occupied;
		is_occupied = nullptr;
	}

private:

	vec2f convertToGrid(const vec2f& position)const;
	vec2f convertToPixel(const vec2f& position)const;

private:

	Grid(const Grid&) = delete;
	Grid(Grid&&) = delete;
	Grid& operator=(const Grid&) = delete;
};


