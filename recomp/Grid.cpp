#include "Grid.h"



bool InstancedGrid::isFilled(const vec2i& position) {
	assert(position.x >= 0 && position.x < width && position.y >= 0 && position.y < height);//just crash because code err
	return is_occupied[position.y * width + position.x];
}

vec2i InstancedGrid::getNext(const vec2i& currentPos, const Direction direction) {
	vec2i next = convertToGrid(currentPos) + getMoveDirection(direction);
	return convertToPixel(next);
}

void InstancedGrid::setTile(const vec2i& position, const SetTile state) {//0 being free it, 1 being fill it
	assert(position.x >= 0 && position.x < width && position.y >= 0 && position.y < height);//just crash because code err
	is_occupied[position.y * width + position.y] = state;
}

vec2i InstancedGrid::getMoveDirection(const Direction direction) {
	switch (direction) {
	case Direction::up:	       return { 0,-1 };
	case Direction::down:	   return { 0,1 };
	case Direction::left:	   return { -1,0 };
	case Direction::right:	   return { 1,0 };
	}

	throw "blyat";
}



vec2i InstancedGrid::convertToGrid(const vec2i& position) {
	return { position.x / tile_w, position.y / tile_h };
}
vec2i InstancedGrid::convertToPixel(const vec2i& position) {
	return{ position.x * tile_w , position.y * tile_h };
}