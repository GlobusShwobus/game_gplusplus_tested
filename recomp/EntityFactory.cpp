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

		if (!definedEntityType(entityType)) {
			printf("entity type not defined : <%s> <%d>\n", entry["type"].get<std::string>().c_str(), entityType);
			continue;
		}

		switch (entityType) {
		case EntityType_ENEMY:
			initEntityData(enemyData, entry, renderer);
			break;
		case EntityType_PLAYER:
			initEntityData(playerData, entry, renderer);
			break;
			//others cases, like buildings, waterfalls, items GOES HERE
		default:
			printf("unknown type: <%s> <%d>", entry["type"].get<std::string>().c_str(), entityType);
			continue;
		}
	}
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

	std::map<AnimID, AnimationReel> reelCollection;//autismus maximus

	for (const auto& data : animationData) {

		AnimationReel clip;
		
		if (!data.contains("clip_id")) {
			printf("data missing clip_id: <%d>", entityID);
			continue;
		}
		AnimID id = HASH(data["clip_id"].get<std::string>().c_str());

		clip.beginX = data.value("beginX", 0.f);
		clip.beginY = data.value("beginY", 0.f);
		clip.frameCount = data.value("frameCount", 1);
		clip.isLooping = data.value("loops", false);

		reelCollection.emplace(id, std::move(clip));
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
	for (auto& [id, texture] : textureComponents) {
		SDL_DestroyTexture(texture);
	}
	textureComponents.clear();
	animationComponents.clear();
	enemyData.clear();
	playerData.clear();
}