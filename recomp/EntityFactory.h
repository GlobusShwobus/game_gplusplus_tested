#pragma once

#include <map>

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "json.hpp"
#include "BasicComponents.h"

class EntityFactory {

	std::map<EntityID, Sprite> spriteComponents;
	std::map<EntityID, std::vector<AnimationReel>> animationComponents;

	std::map<EnemyID, EnemyData> enemyData;
	std::map<PlayerID, PlayerData> playerData;

	void initEntityEnemy(const nlohmann::json* const enemyData, const EnemyID id);
	void initEntityPlayer(const nlohmann::json* const enemyData, const EnemyID id);
	void initSprite(const nlohmann::json* const spriteData, SDL_Renderer* renderer, const EntityID id);
	void initAnimations(const nlohmann::json* const animationData, const EnemyID id);

public:

	//animations where?
	EntityFactory(const nlohmann::json* const entityConfig, SDL_Renderer* renderer);

	//caller is the owner
	Player* createPlayer(const char* type);


	~EntityFactory();
public:
	EntityFactory(const EntityFactory&) = delete;
	EntityFactory(EntityFactory&&)noexcept = delete;
	EntityFactory& operator=(const EntityFactory&) = delete;
};

