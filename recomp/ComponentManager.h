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
* Grid class to become GameScene class
* GameScene class stores and handles ownership of entities (which are to be created by 'new' via EntityFactory)
* 
* Current issue: don't like how sprite component stored animation data, desire to store it independently, maybe in entities

#######################################################################################################################
*/

class ComponentManager {

	std::map<SpriteID, Sprite> spriteComponents;
	std::map<ClipID, Animation> animationComponents;

public:

	EntityManager(const nlohmann::json* const metaData, const nlohmann::json* const animationData, SDL_Renderer* renderer) {
		try {
			bootUpSprites(renderer, metaData);
			bootUpAnimations(animationData);
		}
		catch (const std::exception& e) {
			printf("\nEntityManager BootUp failure: %s\n", e.what());
			std::exit(EXIT_FAILURE);
		}
	}


	//makes a copy
	Sprite createSprite(const SpriteID spriteID);
	~TextureManager();

private:
	void bootUpSprites(SDL_Renderer* renderer, const nlohmann::json* const data);
	void bootUpAnimations(const nlohmann::json* const data);
public:
	TextureManager(const TextureManager&) = delete;
	TextureManager(TextureManager&&)noexcept = delete;
	TextureManager& operator=(const TextureManager&) = delete;
};

