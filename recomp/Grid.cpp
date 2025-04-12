#include "Grid.h"

//#include <iostream> 

bool InstancedGrid::isFilled(int cord_x, int cord_y) {
	assert(cord_x >= 0 && cord_x < width && cord_y >= 0 && cord_y < height);//just crash because code err
	return is_occupied[cord_y * width + cord_x];
}

bool InstancedGrid::isNextFilled(int currPixel_x, int currPixel_y, NextTile move_dir) {
	vec2i gridpos = pixelToGrid(currPixel_x, currPixel_y);
	vec2i nexttile = gridpos + getMoveDirection(move_dir);

	if (nexttile.x < 0 || nexttile.x >= width || nexttile.y < 0 || nexttile.y >= height) {
		return true;
	}

	return is_occupied[nexttile.y * width + nexttile.x];
}

void InstancedGrid::setTile(int grid_x, int grid_y, TileState state) {//0 being free it, 1 being fill it
	assert(grid_x >= 0 && grid_x < width && grid_y >= 0 && grid_y < height);//just crash because code err
	is_occupied[grid_y * width + grid_x] = state;
}
void InstancedGrid::setNextTile(NextTile move_dir, SDL_FRect* to_update) {//0 being free it, 1 being fill it
	vec2i gridpos = pixelToGrid(to_update->x, to_update->y);
	vec2i nexttile = gridpos + getMoveDirection(move_dir);

	if (nexttile.x < 0 || nexttile.x >= width || nexttile.y < 0 || nexttile.y >= height) {
		return;
	}

	setTile(gridpos.x, gridpos.y, TileState_free);

	is_occupied[nexttile.y * width + nexttile.x] = TileState_fill;

	//update the entity because yeah

	vec2f newPixelPos = gridToPixel(nexttile.x, nexttile.y);
	to_update->x = newPixelPos.x;
	to_update->y = newPixelPos.y;
}

vec2i InstancedGrid::getMoveDirection(NextTile move_dir) {
	switch (move_dir) {
	case up:	   return { 0,-1 };
	case down:	   return { 0,1 };
	case left:	   return { -1,0 };
	case right:	   return { 1,0 };
	}
	return { 0,0 };//shit but don't move
}
vec2i InstancedGrid::pixelToGrid(int pixel_x, int pixel_y) {
	return { pixel_x / tile_w, pixel_y / tile_h };
}
vec2f InstancedGrid::gridToPixel(int grid_x, int grid_y) {
	return{ (float)grid_x * tile_w , (float)grid_y * tile_h };
}
int InstancedGrid::getW()const {
	return width;
}
int InstancedGrid::getH()const {
	return height;
}