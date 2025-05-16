#pragma once

#include <map>

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "json.hpp"
#include "BasicComponents.h"

/*
#######################################################################################################################

* EntityFactory stores all building blocks requires to create entities
* EntityFactory creates entities from blocks of data stored into it
* EntityFactory handles owvership of components
* Entities may store, but should never own, pointers to xyz
* All data required to build entities is read at the start of the program from jsons and stored into memory
* 
* PLAYER IS SPECIAL CASE HANDELED SEPARATELY
* 
* window init unique json
* animation data unique json
* entities data unique json
* player unique json (also contains animation information)
* 
* 
* Grid class to become GameScene class
* GameScene class stores and handles ownership of entities (which are to be created by 'new' via EntityFactory)
* 
* Current issue: don't like how sprite component stored animation data, desire to store it independently, maybe in entities

#######################################################################################################################
*/

class EntityFactory {

	std::map<EntityID, Sprite> spriteComponents;
	std::map<EntityID, std::vector<FrameReelData>> animationComponents;

	std::map<EnemeyID, EnemyData> enemyData;

public:

	EntityFactory() {}

	void bootUpSprites(SDL_Renderer* renderer, const nlohmann::json* const data);
	void bootUpAnimations(const nlohmann::json* const data);

	//makes a copy
	Sprite createSprite(const SpriteID spriteID);

	~EntityFactory();

public:
	EntityFactory(const EntityFactory&) = delete;
	EntityFactory(EntityFactory&&)noexcept = delete;
	EntityFactory& operator=(const EntityFactory&) = delete;
};

