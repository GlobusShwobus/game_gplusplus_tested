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

		std::map<HKey::ENTITY_TYPE, SDL_Texture*>             textures;
		std::map<HKey::ENTITY_TYPE, TSA::SpriteStriped>       spriteStriped;		
		std::map<HKey::ENTITY_TYPE, std::vector<TSA::Reel>>   animations;
		std::map<HKey::ENTITY_TYPE, CCP::HitBox>              hitboxes;


		void initAnimations(const nlohmann::json& animationData, const EntityGeneric entityID);
		void initEntityData(std::map<EntityGeneric, EntityData>& container, const nlohmann::json& entityData, SDL_Renderer* renderer);


		void componentConstructionTable(const nlohmann::json& data, SDL_Renderer* renderer, const HKey::ENTITY_TYPE key, const std::string& keyStr) {
			
			if (data.contains("texture_path")) {
				if (constructSpriteTexture(data, renderer, key, keyStr)) {
					constructSpriteRectStriped(data, key);
				}
			}
			else {
				printf("\nwarning: entity construction without texture: <%s>", keyStr);
			}

			if (data.contains("animations")) {
				constructAnimationCoordinates(data["animations"], key);
			}
			else {
				printf("\nwarning: entity construction without animation: <%s>", keyStr);
			}



		}

		bool constructSpriteTexture(const nlohmann::json& data, SDL_Renderer* renderer, const HKey::ENTITY_TYPE key, const std::string& keyStr)
		{	
			std::string path = data["texture_path"].get<std::string>();
			SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
			
			if (!texture) {
				printf("\nerror: entity texture missing physical file: <type: %s>\t<path: %s>", path, keyStr);
				return false;
			}

			textures.emplace(key, texture);
			return true;
		}
		void constructSpriteRectStriped(const nlohmann::json& data, const HKey::ENTITY_TYPE key)
		{
			TSA::SpriteStriped ss;

			if (data.contains("texture_src")) 
				ss.src = constructSpriteRectStriped(data, "texture_src");
			else
				printf("\nwarning: missing field rect: <texture_src>");
			

			if (data.contains("texture_dest")) 
				ss.dest = constructSpriteRectStriped(data, "texture_dest");
			else
				printf("\nwarning: missing field rect: <texture_dest>");
			

			spriteStriped.emplace(key, ss);
		}
		SDL_FRect constructSpriteRectStriped(const nlohmann::json& data, const char* field)
		{
			SDL_FRect rect = { 0,0,0,0 };

			try {
				const auto& arect = data[field];
				rect = { arect[0],arect[1] ,arect[2] ,arect[3] };
			}
			catch (std::exception& expt) {
				printf("\nerror: rect creation: <%s>", expt.what());
				rect = { 0,0,0,0 };
			}

			return rect;
		}

		void constructAnimationCoordinates(const nlohmann::json& data, const HKey::ENTITY_TYPE key, const std::string& keyStr)
		{
			std::vector<TSA::Reel> animationCollections;

			for (const auto& rData : data) {

				if (!rData.contains("id") || !rData.contains("reel_xyw") && rData["reel_xyw"].is_object()) {
					printf("\nerror: animation reel missing id or invalid reel setup: <%s>", keyStr);
					continue;
				}

				TSA::Reel clip;

				std::string aids = data["id"].get<std::string>();//animation id string
				clip.id = HKey::buildAnimationID(aids.c_str());
				clip.isLooping = data.value("loops", false);

				const int frameCount = data.value("frameCount", 1);
				
				try {
					const int x = rData["reel_xyw"][0];
					const int y = rData["reel_xyw"][1];
					const int w = rData["reel_xyw"][2];

					for (int i = 0; i < frameCount; i++) {
						SDL_FPoint frameCordinate;
						frameCordinate.x = x + (i * w);
						frameCordinate.y = y;

						clip.frames.push_back(std::move(frameCordinate));
					}

				}
				catch (std::exception& expt) {
					printf("\nerror: aniamtion clip invalid index: <%s>\t<%s>", keyStr, aids);
					continue;
				}

				animationCollections.push_back(std::move(clip));
			}

			animations.emplace(key, std::move(animationCollections));
		}

	public:

		EntityFactory() = default;

		bool componentInitalizationPhase(const nlohmann::json* const entityConfig, SDL_Renderer* renderer);

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