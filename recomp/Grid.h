#pragma once

#include <assert.h>
#include "vec2.h"
#include "SDL3/SDL.h"

//need inforamtion save/laod feature using binary files

enum TileState {
	TileState_fill = 1,
	TileState_free = 0
};

enum NextTile {
	up = SDL_SCANCODE_W,
	down = SDL_SCANCODE_S,
	left = SDL_SCANCODE_A,
	right = SDL_SCANCODE_D
};

class InstancedGrid {

	static constexpr int tile_w = 32;
	static constexpr int tile_h = 32;

	bool* is_occupied = nullptr;
	int width = 0;
	int height = 0;

	vec2i GetMoveDir(NextTile move_dir);

	vec2i PixelToGrid(int x, int y);
	vec2f GridToPixel(int grid_x, int grid_y);


public:

	InstancedGrid(int image_width, int image_height) : width(image_width / tile_w), height(image_height / tile_h) {

		is_occupied = new bool[width * height] {false};

	}

	bool IsFilled(int x, int y);
	bool isNextFilled(int curr_x, int curr_y, NextTile move_dir);

	void SetTile(int x, int y, TileState state); //0 being free it, 1 being fill it
	void SetNextTile(NextTile move_dir, SDL_FRect* to_update); //0 being free it, 1 being fill it


	~InstancedGrid() {
		delete[] is_occupied;
		is_occupied = nullptr;
	}

public:
	//garbage for testing, depricate sometime (tm)
	int getW()const;
	int getH()const;

private:
	InstancedGrid(const InstancedGrid&) = delete;
	InstancedGrid(InstancedGrid&&) = delete;
	InstancedGrid& operator=(const InstancedGrid&) = delete;
};


