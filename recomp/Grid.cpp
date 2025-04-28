#include "Grid.h"

bool Grid::isValid(const vec2f& position)const {
	return (position.x >= 0 && position.x < (pixelsInX - tileWidth)) &&
		(position.y >= 0 && position.y < (pixelsInY - tileHeight));
}
Grid::Tile& Grid::getTile(const vec2f& position) {
	const int atX = position.x / tileWidth;
	const int atY = position.y / tileHeight;

	return tiles[atY * tilesInX + atX];
}

//grid isValid only makes sure the texture is within the bounds of the map, nothing more
//it is not able to check the bounds of any tile inside the map, only access them

//tile class does not care where it exists, it simply holds description of the tile

//i need to be able check if the next tile, from the textures right and bottom, to tiles top and left
//AND i need to check if the next tile, from the textures top and left, to tiles bottom and right is not walkable


void Grid::Tile::addData(const TFLAG data) {
	tileData |= data;
}
void Grid::Tile::removeData(const TFLAG data) {
	tileData &= ~data;
}
bool Grid::Tile::doesContain(const TFLAG data)const {
	return tileData & data;
}
bool Grid::Tile::isWalkable()const {
	return tileData & TFLAG_WALKABLE;
}
