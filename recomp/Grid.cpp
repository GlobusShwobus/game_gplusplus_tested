#include "Grid.h"

#include <iostream> 
bool InstancedGrid::IsFilled(int x, int y) {
	assert(x >= 0 && x < width && y >= 0 && y < height);//just crash because code err
	return is_occupied[y * width + x];
}
bool InstancedGrid::isNextFilled(int curr_x, int curr_y, NextTile move_dir) {
	vec2i gridpos = PixelToGrid(curr_x, curr_y);
	vec2i nexttile = gridpos + GetMoveDir(move_dir);

	if (nexttile.x < 0 || nexttile.x >= width || nexttile.y < 0 || nexttile.y >= height) {
		return true;
	}

	return is_occupied[nexttile.y * width + nexttile.x];
}

void InstancedGrid::SetTile(int x, int y, TileState state) {//0 being free it, 1 being fill it
	assert(x >= 0 && x < width && y >= 0 && y < height);//just crash because code err
	is_occupied[y * width + x] = state;
}
void InstancedGrid::SetNextTile(NextTile move_dir, SDL_FRect* to_update) {//0 being free it, 1 being fill it
	vec2i gridpos = PixelToGrid(to_update->x, to_update->y);
	vec2i nexttile = gridpos + GetMoveDir(move_dir);

	if (nexttile.x < 0 || nexttile.x >= width || nexttile.y < 0 || nexttile.y >= height) {
		return;
	}

	SetTile(gridpos.x, gridpos.y, TileState_free);

	is_occupied[nexttile.y * width + nexttile.x] = TileState_fill;

	//update the entity because yeah

	vec2f newPixelPos = GridToPixel(nexttile.x, nexttile.y);
	to_update->x = newPixelPos.x;
	to_update->y = newPixelPos.y;
}

vec2i InstancedGrid::GetMoveDir(NextTile move_dir) {
	switch (move_dir) {
	case up:	   return { 0,-1 };
	case down:	   return { 0,1 };
	case left:	   return { -1,0 };
	case right:	   return { 1,0 };
	}
	return { 0,0 };//shit but don't move
}
vec2i InstancedGrid::PixelToGrid(int x, int y) {
	return { x / tile_w, y / tile_h };
}
vec2f InstancedGrid::GridToPixel(int grid_x, int grid_y) {
	return{ (float)grid_x * tile_w , (float)grid_y * tile_h };
}
int InstancedGrid::getW()const {
	return width;
}
int InstancedGrid::getH()const {
	return height;
}