#pragma once


#include "SDL3/SDL.h"
#include "vec2.h"


enum TileDataFlags {
	TFLAG_EMPTY    = 1 << 0,
	TFLAG_WALKABLE = 1 << 1,
};
typedef int TFLAG;

class Grid {

	static constexpr int tileWidth = 32;
	static constexpr int tileHeight = 32;

	const int tilesInX = 0;
	const int tilesInY = 0;

	const int pixelsInX = 0;
	const int pixelsInY = 0;


	class Tile {
	public:
		Tile() = default;

		void addData(const TFLAG data);
		void removeData(const TFLAG data);
		bool doesContain(const TFLAG data)const;
	private:
		int tileData = TFLAG_EMPTY | TFLAG_WALKABLE;
	};

	Tile* tiles = nullptr;

public:

	Grid(const int mapWidth, const int mapHeight) : tilesInX(mapWidth / tileWidth), tilesInY(mapHeight / tileHeight), pixelsInX(mapWidth), pixelsInY(mapHeight){
		tiles = new Tile[tilesInX * tilesInY]();
	}

	bool isValidTile(const SDL_FPoint& point)const;
	Tile& getTile(const SDL_FPoint& point);
	
	~Grid() {
		delete[] tiles;
		tiles = nullptr;
	}

private:

	Grid(const Grid&) = delete;
	Grid(Grid&&) = delete;
	Grid& operator=(const Grid&) = delete;
};


