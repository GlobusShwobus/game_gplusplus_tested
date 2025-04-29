#include "Grid.h"

bool Grid::isValidTile(const SDL_FPoint& point)const {
	return (point.x >= 0 && point.x < pixelsInX) &&
		(point.y >= 0 && point.y < pixelsInY);
}

Grid::Tile& Grid::getTile(const SDL_FPoint& point) {
	const int atX = point.x / tileWidth;
	const int atY = point.y / tileHeight;

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
