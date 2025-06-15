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

		std::map<EntityCategoryType, SDL_Texture*> textureComponents;//owner of the textures returned by IMG_LoadTexture and cleaned up at factory destructor
		std::map<EntityCategoryType, std::vector<FrameMap>> animationComponents;

		std::map<EDM::EnemyType, EDM::EnemyBasic> enemyTemplates;
		std::map<EDM::PlayerType , EDM::Player> playerTemplates;

		void initAnimations(const nlohmann::json& animationData, const EntityGeneric entityID);
		void initEntityData(std::map<EntityGeneric, EntityData>& container, const nlohmann::json& entityData, SDL_Renderer* renderer);

		void createEntry(const nlohmann::json& entry, HKey::ENTITY_CATEGORY_HKEY key, SDL_Renderer* renderer){
			switch (key) {
			case HKey::ENTITY_TYPE_PLAYER:
				break;
			case HKey::ENTITY_TYPE_ENEMY:
				break;
			default:
				printf("unknown type: <%s> <%d>", entry["type"].get<std::string>().c_str(), key);
				break;
			}
		}
		void newPlayerEntry(const nlohmann::json& pData, SDL_Renderer* renderer) {

		}
	public:

		EntityFactory() = default;

		bool initFactory(const nlohmann::json* const entityConfig, SDL_Renderer* renderer) {
			if (!entityConfig) {
				printf("\nentityConfig nullptr, cannot initalize components\n");
				return false;
			}

			for (const auto& entry : *entityConfig) {
				if (!entry.contains("type") || !entry.contains("entity_id")) {
					printf("entity missing type or id\n");
					continue;
				}
				const HKey::ENTITY_CATEGORY_HKEY category = HKey::createHashKey(entry["type"].get<std::string>().c_str());

				if (!HKey::definedEntityCategory(category)) {
					printf("entity type not defined : <%s> <%d>\n", entry["type"].get<std::string>().c_str(), category);
					continue;
				}

			}

			return true;
		}

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