#include "Grid.h"



bool Grid::isFilled(const vec2f& position)const {
	vec2f tile = convertToGrid(position);

	if (tile.x < 0 || tile.x >= width || tile.y < 0 || tile.y >= height) {//edges
		return true;
	}

	return is_occupied[int(tile.y * width + tile.x)];
}

void Grid::setTile(const vec2f& position, const SetTile state) {//0 being free it, 1 being fill it
	vec2f tile = convertToGrid(position);	
	is_occupied[int(tile.y * width + tile.x)] = state;
}

vec2f Grid::convertToGrid(const vec2f& position)const {
	return { position.x / tile_w, position.y / tile_h };
}
vec2f Grid::convertToPixel(const vec2f& position)const {
	return { position.x * tile_w , position.y * tile_h };
}