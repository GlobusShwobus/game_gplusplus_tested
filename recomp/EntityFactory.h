#pragma once

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "json.hpp"
#include "HashKeys.h"
#include "EntityData.h"

/*
* Data initalized by the factory should NEVER be cleaned up during runtime as entities will depend on the basic components
*/
namespace badEngine {

	class EntityFactory {

		std::map<HKey::ENTITY_TYPES, TSA::Sprite> sprites;//owner of the textures returned by IMG_LoadTexture and cleaned up at factory destructor
		std::map<HKey::ENTITY_TYPES, std::vector<TSA::Reel>> animations;

		std::map<HKey::ENTITY_TYPES, EDM::EnemyBasic> enemyTemplates;
		std::map<HKey::ENTITY_TYPES, EDM::Player> playerTemplates;

		void initAnimations(const nlohmann::json& animationData, const EntityGeneric entityID);
		void initEntityData(std::map<EntityGeneric, EntityData>& container, const nlohmann::json& entityData, SDL_Renderer* renderer);

	public:

		EntityFactory() = default;

		bool initFactory(const nlohmann::json* const entityConfig, SDL_Renderer* renderer);

		//caller is the owner, can return nullptr
		Player* createPlayer(const PlayerID id)const;
		//caller is the owner, can return nullptr
		EnemyBasic* createEnemy(const EnemyID id)const;

		~EntityFactory();
	public:
		EntityFactory(const EntityFactory&) = delete;
		EntityFactory(EntityFactory&&)noexcept = delete;
		EntityFactory& operator=(const EntityFactory&) = delete;
		EntityFactory& operator=(EntityFactory&&)noexcept = delete;
	};

}