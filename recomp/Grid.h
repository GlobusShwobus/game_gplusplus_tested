#pragma once


#include "SDL3/SDL.h"
#include "vec2.h"

/*

* Grid class to become the scene class
* it is important to retain the functionality of the grid for more linear collision checking
* it is important to retain the functionality of the grid for invisible walls (unwalkable terrain)
* Scene class must read data for a scene from a config
*	currently planned is holding the mapping data (coordinates which are walkable which not)
*	could also hold information for scrips, but not that far currently
* Scene class is also the owner of entities
*	does not create entities
*	contains and manages life cycle
* upon scene change, it may be fine to clean up entities, but would be good to store data which remembers where what to spawn (again, a bit far a head)
* 
* Tile object to become a basic bool and nothing else which tells the tile is walkable or not
* HARD PART, how tf does a tile/cell know what entities stand in there

*/

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


