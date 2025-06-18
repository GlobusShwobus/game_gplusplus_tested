#include "EntityFactory.h"
namespace badEngine {
	bool EntityFactory::componentInitalizationPhase(const nlohmann::json* const entityConfig, SDL_Renderer* renderer) {
		if (!entityConfig) {//if null ptr, kill it
			printf("\nentityConfig nullptr, cannot initalize components\n");
			return false;
		}

		for (const auto& entry : *entityConfig) {

			if (!entry.contains("type")) {
				printf("\nentity missing type specification");
				continue;
			}
			std::string keyStr = entry["type"].get<std::string>();
			HKey::ENTITY_TYPE key = HKey::buildEntityTypeID(keyStr.c_str());

			if (!HKey::isValidID(key)) {
				printf("\ninvalid entity type unknown");
				continue;
			}

			componentConstructionTable(entry, renderer, key);
		}
		return true;
	}
	void EntityFactory::initEntityData(std::map<EntityGeneric, EntityData>& container, const nlohmann::json& entityData, SDL_Renderer* renderer) {

		EntityData entry;

		const EntityGeneric entityID = HASH(entityData["entity_id"].get<std::string>().c_str());

		entry.id = entityID;
		entry.movement_speed = entityData.value("movement_speed", 0.f);
		entry.health_points = entityData.value("health_points", 1.f);
		entry.frameWidth = entityData.value("frameWidth", 32.f);
		entry.frameHeight = entityData.value("frameHeight", 32.f);
		entry.mass = entityData.value("mass", 0.f);

		if (entityData.contains("transform")) {
			auto& transform = entityData["transform"];
			entry.transform = Transform(transform["x"], transform["y"], transform["w"], transform["h"]);
		}
		if (entityData.contains("texturePath")) {
			SDL_Texture* texture = IMG_LoadTexture(renderer, entityData["texturePath"].get<std::string>().c_str());
			textureComponents.emplace(entityID, texture);
		}
		if (entityData.contains("animations")) {
			initAnimations(entityData["animations"], entityID);
		}

		container.emplace(entityID, std::move(entry));
	}
	void EntityFactory::initAnimations(const nlohmann::json& animationData, const EntityGeneric entityID) {

		std::vector<FrameMap> reelCollection;//autismus maximus
		reelCollection.reserve(16);//idk, is 16 is good for now

		for (const auto& data : animationData) {
			FrameMap clip;

			clip.id = HASH(data["clip_id"].get<std::string>().c_str());
			clip.isLooping = data.value("loops", false);

			const int frameCount = data["frameCount"];

			for (int i = 0; i < frameCount; i++) {

				int x = data["x"];
				int y = data["y"];
				int w = data["w"];
				int h = data["h"];

				SDL_Rect frame{ x + (i * w), y,w,h };

				clip.frames.emplace_back(std::move(frame));
			}

			reelCollection.emplace_back(std::move(clip));
		}
		reelCollection.shrink_to_fit();
		animationComponents.emplace(entityID, std::move(reelCollection));
	}

	Player* EntityFactory::createPlayer(const PlayerID id)const {
		Player* player = nullptr;

		if (textureComponents.contains(id) && animationComponents.contains(id) && playerData.contains(id)) {
			player = new Player(textureComponents.at(id), animationComponents.at(id), playerData.at(id));
		}
		else {
			printf("something wrong in player data initialization OR invalid ID\n");
		}
		return player;
	}
	EnemyBasic* EntityFactory::createEnemy(const EnemyID id)const {
		EnemyBasic* enemybasic = nullptr;

		if (textureComponents.contains(id) && enemyData.contains(id)) {
			enemybasic = new EnemyBasic(textureComponents.at(id), enemyData.at(id));
		}
		else {
			printf("something wrong in enemyBasic data initialization OR invalid ID\n");
		}
		return enemybasic;
	}

	EntityFactory::~EntityFactory()
	{
		for (auto& [id, texture] : textureComponents) {
			SDL_DestroyTexture(texture);
		}
		textureComponents.clear();
		animationComponents.clear();
		enemyData.clear();
		playerData.clear();
	}
}