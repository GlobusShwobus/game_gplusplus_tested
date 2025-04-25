#include "Grid.h"



bool Grid::isFilled(const vec2f& position)const {
	if (position.x < 0 || position.x >= width || position.y < 0 || position.y >= height) {//edges
		return true;
	}
	return is_occupied[int(position.y * width + position.x)];
}

void Grid::setTile(const vec2f& position, const SetTile state) {//0 being free it, 1 being fill it
	is_occupied[int(position.y * width + position.x)] = state;
}

vec2f Grid::getDirection(const Direction direction)const {
	switch (direction) {
	case Direction::up:	       return { 0,-1 };
	case Direction::down:	   return { 0,1 };
	case Direction::left:	   return { -1,0 };
	case Direction::right:	   return { 1,0 };
	}

	throw "blyat";
}
vec2f Grid::convertToGrid(const vec2f& position)const {
	return { position.x / tile_w, position.y / tile_h };
}
vec2f Grid::convertToPixel(const vec2f& position)const {
	return{ position.x * tile_w , position.y * tile_h };
}