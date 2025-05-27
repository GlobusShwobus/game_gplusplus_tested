#pragma once

#include <map>

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "json.hpp"
#include "BasicComponents.h"

class EntityFactory {

	std::map<EntityGeneric, TextureData> textureComponents;//owner of the textures returned by IMG_LoadTexture and cleaned up at factory destructor
	std::map<EntityGeneric, std::vector<AnimationReel>> animationComponents;

	std::map<EnemyID, EntityData> enemyData;
	std::map<PlayerID, EntityData> playerData;

	void initTexture(const nlohmann::json& textureData, SDL_Renderer* renderer, const EntityGeneric entityID);
	void initAnimations(const nlohmann::json& animationData, const EntityGeneric entityID);

	void initEntityData(std::map<EntityGeneric, EntityData>& container, const nlohmann::json& entityData, const EntityGeneric entityID);
public:

	EntityFactory(const nlohmann::json* const entityConfig, SDL_Renderer* renderer);

	//caller is the owner, can return nullptr
	Player* createPlayer(const char* type);
	//caller is the owner, can return nullptr
	EnemyBasic* createEnemy(const char* type);

	~EntityFactory();
public:
	EntityFactory(const EntityFactory&) = delete;
	EntityFactory(EntityFactory&&)noexcept = delete;
	EntityFactory& operator=(const EntityFactory&) = delete;
};

