#include "EntityFactory.h"

EntityFactory::EntityFactory(const nlohmann::json* const entityConfig, SDL_Renderer* renderer) {
	if (!entityConfig) {//if null ptr, kill it
		throw std::logic_error("\nentityConfig nullptr, cannot initalize components\n");
	}

	for (const auto& entry : *entityConfig) {

		if (!entry.contains("type") || !entry.contains("entity_id")) {
			printf("entity missing type or id\n");
			continue;
		}

		const EntityType entityType = HASH(entry["type"].get<std::string>().c_str());
		const EntityGeneric entityID = HASH(entry["entity_id"].get<std::string>().c_str());

		if (!definedEntityType(entityType)) {
			printf("entity type not defined : <%s> <%d>\n", entry["type"].get<std::string>().c_str(), entityType);
			continue;
		}

		switch (entityType) {
		case EntityType_ENEMY:
			initEntityData(enemyData, entry, entityID);
			break;
		case EntityType_PLAYER:
			initEntityData(playerData, entry, entityID);
			break;
			//others cases, like buildings, waterfalls, items GOES HERE
		default:
			printf("unknown type: <%s> <%d>", entry["type"].get<std::string>().c_str(), entityType);
			continue;
		}

		//not every entity may have a sprite, maybe idk
		if (entry.contains("sprite")) {
			initTexture(entry["sprite"], renderer, entityID);
		}
		//not every entity may be animatable, but the clip data is structured the same way
		if (entry.contains("animations")) {
			initAnimations(entry["animations"], entityID);
		}
	}
}
void EntityFactory::initEntityData(std::map<EntityGeneric, EntityData>& container, const nlohmann::json& entityData, const EntityGeneric entityID) {

	EntityData entry;

	entry.id = entityID;
	entry.movement_speed = entityData.value("movement_speed", 0.f);
	entry.health_points = entityData.value("health_points", 1.f);
	entry.mass = entityData.value("mass", 0.f);

	if (entityData.contains("transform")) {
		auto& transform = entityData["transform"];
		entry.transform = Transform(transform["x"], transform["y"], transform["w"], transform["h"]);
	}
	container.emplace(entityID, std::move(entry));
}

void EntityFactory::initTexture(const nlohmann::json& textureData, SDL_Renderer* renderer, const EntityGeneric entityID) {

	if (!textureData.contains("texture_path") || !textureData.contains("texture_source") || !textureData.contains("texture_destination")) {
		printf("texture lacks all required data: <%d>", entityID);
		return;
	}

	const std::string texturePath = textureData["texture_path"];
	auto& sourceData = textureData["texture_source"];
	auto& destData = textureData["texture_destination"];

	SDL_Texture* texture = IMG_LoadTexture(renderer, texturePath.c_str());

	if (!texture) {
		printf("texture path invalid or texture at path not found: <%s>", texturePath.c_str());
		return;
	}

	TextureData tData;
	tData.texture = texture;
	tData.source = { sourceData[0],sourceData[1], sourceData[2], sourceData[3] };
	tData.destination = { destData[0],destData[1], destData[2], destData[3] };

	textureComponents.emplace(entityID, std::move(tData));
}
void EntityFactory::initAnimations(const nlohmann::json& animationData, const EntityGeneric entityID) {

	std::vector<AnimationReel> reelCollection;//autismus maximus

	for (const auto& data : animationData) {

		AnimationReel clip;
		
		if (!data.contains("clip_id")) {
			printf("data missing clip_id: <%d>", entityID);
			continue;
		}
		clip.id = HASH(data["clip_id"].get<std::string>().c_str());
		clip.initialFrame.x = data.value("x", 0.f);
		clip.initialFrame.y = data.value("y", 0.f);
		clip.initialFrame.w = data.value("width", 32.f);//need a better way for max width/height
		clip.initialFrame.h = data.value("height", 32.f);//need a better way for max width/height
		clip.frameCount = data.value("frame_count", 1);
		clip.frameDelay = data.value("frame_delay", 10000);
		clip.isLooping = data.value("loops", false);
		reelCollection.push_back(std::move(clip));
	}

	animationComponents.emplace(entityID, std::move(reelCollection));
}

Player* EntityFactory::createPlayer(const char* type) {
	PlayerID id = HASH(type);
	Player* player = nullptr;

	if (textureComponents.contains(id) && animationComponents.contains(id) && playerData.contains(id)) {
		player = new Player(textureComponents[id], animationComponents[id], playerData[id]);
	}
	else {
		printf("something wrong in player data initialization: missing data\n");
	}
	return player;
}
EnemyBasic* EntityFactory::createEnemy(const char* type) {
	EnemyID id = HASH(type);
	EnemyBasic* enemybasic = nullptr;
	
	if (textureComponents.contains(id) && enemyData.contains(id)) {
		enemybasic = new EnemyBasic(textureComponents[id], enemyData[id]);
	}
	else {
		printf("something wrong in enemyBasic data initialization: missing data\n");
	}
	return enemybasic;
}

EntityFactory::~EntityFactory()
{
	for (auto& [id, sprite] : textureComponents) {
		SDL_DestroyTexture(sprite.texture);
	}
	textureComponents.clear();
	animationComponents.clear();
	enemyData.clear();
	playerData.clear();
}