#pragma once

#include <map>

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "json.hpp"
#include "BasicComponents.h"

/*
#######################################################################################################################

* EntityManager stores all components
* EntityManager stores entities
* Entities are all base objects(or normal no virtual inheritence, will see)
* EntityManager stores all entity types in independent containers
* All data required to build entities is read at the start of the program from jsons and stored into memory
* When in need to create an entity, do it so via enums and then use the building blocks to create whatever
* 
* Current issue: don't like how sprite component stored animation data, desire to store it independently, maybe in entities
* Current issue: Grid.h is redundant but its functionality valueable, probably merge the tile system into EntityManager

#######################################################################################################################
*/
class Player {

public:

	Sprite sprite;
	Movement movement;
	Camera camera;

	static constexpr float speed = 2.5f;//uhm, speed*FPS is real speed so yeah. oops

	Player(Sprite texture, SDL_FPoint cameraRadii) :sprite(texture), camera(cameraRadii.x, cameraRadii.y) {}
};

class EntityManager {

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

