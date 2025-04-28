#pragma once

#include <assert.h>
#include "vec2.h"

typedef bool SetTile;
typedef bool WalkAbleState;

enum class Direction {	//yeet this function in misc pile
	up,
	down,
	left,
	right,
};

/*

NOTE TO SELF!!!

There is a missmatch between tile positions and real pixel positions, how SDL_FRect is floats so it's simple to just work with floats
This means there is a truncation issue. Currently it works as intended by truncating floats to integers and accessing correct tiles since floats trunc in one direction

But there is always an invisible asshole (potential) bug to keep in mind

*/

enum TileDataFlags {
	TFLAG_EMPTY = 1 << 0,
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
		bool isWalkable()const;
	private:
		int tileData = TFLAG_EMPTY | TFLAG_WALKABLE;
	};

	
	Tile* tiles = nullptr;


public:

	Grid(const int mapWidth, const int mapHeight) : tilesInX(mapWidth / tileWidth), tilesInY(mapHeight / tileHeight), pixelsInX(mapWidth), pixelsInY(mapHeight){
		tiles = new Tile[tilesInX * tilesInY]();
	}

	bool isValid(const vec2f& position)const;
	Tile& getTile(const vec2f& position);

	~Grid() {
		delete[] tiles;
		tiles = nullptr;
	}

private:

	Grid(const Grid&) = delete;
	Grid(Grid&&) = delete;
	Grid& operator=(const Grid&) = delete;

};

