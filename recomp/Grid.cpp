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
