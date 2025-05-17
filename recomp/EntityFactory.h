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
	std::map<EntityID, std::vector<AnimationReel>> animationComponents;

	std::map<EnemyID, EnemyData> enemyData;

	void initEnemyData(const nlohmann::json* const enemyData, const EnemyID id) {

		EnemyData entry;

		entry.id = id;
		entry.movement_speed = (*enemyData)["movement_speed"];
		entry.health_points = (*enemyData)["health_points"];
		entry.attack_power = (*enemyData)["attack_power"];
		entry.attack_interval = (*enemyData)["attack_interval"];

		this->enemyData.emplace(entry.id, entry);
	}

	void initSprite(const nlohmann::json* const spriteData, SDL_Renderer* renderer, const EntityID id) {

		const std::string texturePath = (*spriteData)["texture_path"];

		auto& src = (*spriteData)["texture_source"];
		auto& dest = (*spriteData)["texture_destination"];

		SDL_FRect source{ src[0],src[1], src[2], src[3] };
		SDL_FRect destination{ dest[0],dest[1], dest[2], dest[3] };

		SDL_Texture* texture = IMG_LoadTexture(renderer, texturePath.c_str());

		Sprite sprite(texture, &source, &destination);

		spriteComponents.emplace(id, sprite);
	}

public:

	EntityFactory() {}


	void bootUpEntityBuildingBlocks(const nlohmann::json* const entityConfig, SDL_Renderer* renderer) {

		if (!entityConfig) {//if null ptr, kill it
			throw std::logic_error("\nentityConfig nullptr, cannot initalize components\n");
		}

		for (const auto& entry : *entityConfig) {
			
			const std::string entityTypeKey = entry["type"];
			const EntityType type = HASH(entityTypeKey.c_str());
			const std::string entityIDKey = entry["entity_id"];
			const EntityID id = HASH(entityIDKey.c_str());

			const nlohmann::json* const spriteData = &entry["sprite"];
			
			switch (type) {
			case EntityType_ENEMY:
				initEnemyData(&entry, id);
				initSprite(spriteData, renderer, id);
				break;
				//others cases, like buildings, waterfalls, items idk
			default: throw std::logic_error("\ntype not defined\n");
			}
		}
	}


	//makes a copy
	Sprite createSprite(const SpriteID spriteID);

	~EntityFactory();

public:
	EntityFactory(const EntityFactory&) = delete;
	EntityFactory(EntityFactory&&)noexcept = delete;
	EntityFactory& operator=(const EntityFactory&) = delete;
};

