#include "EntityFactory.h"

EntityFactory::EntityFactory(const nlohmann::json* const entityConfig, SDL_Renderer* renderer) {
	if (!entityConfig) {//if null ptr, kill it
		throw std::logic_error("\nentityConfig nullptr, cannot initalize components\n");
	}

	for (const auto& entry : *entityConfig) {

		const std::string entityTypeKey = entry["type"];
		const EntityType type = HASH(entityTypeKey.c_str());
		const std::string entityIDKey = entry["entity_id"];
		const EntityID id = HASH(entityIDKey.c_str());

		const nlohmann::json* const spriteData = &entry["sprite"];

		//entities may have different basic data, hence need for init table
		switch (type) {
		case EntityType_ENEMY:
			initEntityEnemy(&entry, id);
			break;
		case EntityType_PLAYER:
			initEntityPlayer(&entry, id);
			break;
			//others cases, like buildings, waterfalls, items idk
		default: throw std::logic_error("\ntype not defined\n");
		}

		//every entity sprite data is structured the same way
		initSprite(spriteData, renderer, id);

		//not every entity may be animatable, but the clip data is structured the same way
		if (entry.contains("animations")) {
			initAnimations(&entry, id);
		}

	}
}
void EntityFactory::initEntityEnemy(const nlohmann::json* const enemyData, const EnemyID id) {

	EnemyData entry;

	entry.id = id;
	entry.movement_speed = (*enemyData)["movement_speed"];
	entry.health_points = (*enemyData)["health_points"];
	entry.attack_power = (*enemyData)["attack_power"];
	entry.attack_interval = (*enemyData)["attack_interval"];
	this->enemyData.emplace(entry.id, entry);
}
void EntityFactory::initEntityPlayer(const nlohmann::json* const playerData, const EnemyID id) {
	PlayerData entry;

	entry.id = id;
	entry.movement_speed = (*playerData)["movement_speed"];
	entry.health_points = (*playerData)["health_points"];
	entry.attack_power = (*playerData)["attack_power"];

	const auto& transform = (*playerData)["transform"];

	entry.transform = Transform(transform["x"], transform["y"], transform["w"], transform["h"]);

	this->playerData.emplace(entry.id, entry);
}

void EntityFactory::initSprite(const nlohmann::json* const spriteData, SDL_Renderer* renderer, const EntityID id) {

	const std::string texturePath = (*spriteData)["texture_path"];

	auto& src = (*spriteData)["texture_source"];
	auto& dest = (*spriteData)["texture_destination"];

	SDL_FRect source{ src[0],src[1], src[2], src[3] };
	SDL_FRect destination{ dest[0],dest[1], dest[2], dest[3] };

	SDL_Texture* texture = IMG_LoadTexture(renderer, texturePath.c_str());

	TextureData sprite(texture, &source, &destination);

	textureComponents.emplace(id, sprite);
}
void EntityFactory::initAnimations(const nlohmann::json* const animationData, const EnemyID id) {

	std::vector<AnimationReel> reelCollection;//autismus maximus

	for (const auto& data : (*animationData)["animations"]) {

		AnimationReel clip;
		const std::string clipID = data["clip_id"];
		clip.id = HASH(clipID.c_str());
		clip.initialFrame.x = data["x"];
		clip.initialFrame.y = data["y"];
		clip.initialFrame.w = data["width"];
		clip.initialFrame.h = data["height"];
		clip.frameCount = data["frame_count"];
		clip.frameDelay = data["frame_delay"];
		clip.isLooping = data["loops"];

		reelCollection.push_back(clip);
	}

	animationComponents.emplace(id, std::move(reelCollection));
}
EntityFactory::~EntityFactory()
{
	//sprites itself is not the owner of the pointer because many different objects can point to the same texture on the GPU
	//if the entity holding a copy of a sprite, stops existing, then by default it's not the last reference anyway
	//however if the manager stops existing, and this should only happen when the game is closed, that means we must clean up now
	for (auto& [id, sprite] : textureComponents) {
		SDL_DestroyTexture(sprite.texture);
	}
	textureComponents.clear();
	animationComponents.clear();
	enemyData.clear();
	playerData.clear();
}


Player* EntityFactory::createPlayer(const char* type) {
	PlayerID id = HASH(type);

	if (id != PlayerID_Version1) {
		printf("\nillegal type ffor player init: %s\n", type);
	}


	Player* player = new Player(&textureComponents[id], &animationComponents[id], &playerData[id]);

	return player;
}