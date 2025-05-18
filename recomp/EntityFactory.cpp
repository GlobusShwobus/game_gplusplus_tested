#include "EntityFactory.h"

void EntityFactory::bootUpEntityBuildingBlocks(const nlohmann::json* const entityConfig, SDL_Renderer* renderer) {

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
void EntityFactory::initEnemyData(const nlohmann::json* const enemyData, const EnemyID id) {

	EnemyData entry;

	entry.id = id;
	entry.movement_speed = (*enemyData)["movement_speed"];
	entry.health_points = (*enemyData)["health_points"];
	entry.attack_power = (*enemyData)["attack_power"];
	entry.attack_interval = (*enemyData)["attack_interval"];

	this->enemyData.emplace(entry.id, entry);
}

void EntityFactory::initSprite(const nlohmann::json* const spriteData, SDL_Renderer* renderer, const EntityID id) {

	const std::string texturePath = (*spriteData)["texture_path"];

	auto& src = (*spriteData)["texture_source"];
	auto& dest = (*spriteData)["texture_destination"];

	SDL_FRect source{ src[0],src[1], src[2], src[3] };
	SDL_FRect destination{ dest[0],dest[1], dest[2], dest[3] };

	SDL_Texture* texture = IMG_LoadTexture(renderer, texturePath.c_str());

	Sprite sprite(texture, &source, &destination);

	spriteComponents.emplace(id, sprite);
}
EntityFactory::~EntityFactory()
{
	//sprites itself is not the owner of the pointer because many different objects can point to the same texture on the GPU
	//if the entity holding a copy of a sprite, stops existing, then by default it's not the last reference anyway
	//however if the manager stops existing, and this should only happen when the game is closed, that means we must clean up now
	for (auto& [id, sprite] : spriteComponents) {
		SDL_DestroyTexture(sprite.getTexture());
	}
	spriteComponents.clear();
	animationComponents.clear();
	enemyData.clear();
}
