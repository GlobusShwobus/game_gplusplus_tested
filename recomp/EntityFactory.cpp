#include "EntityFactory.h"

void EntityFactory::bootUpSprites(SDL_Renderer* renderer, const nlohmann::json* const data) {
	if (!data) {//if null ptr, kill it
		throw std::logic_error("\nJSON nullptr\n");
	}

	for (const auto& entry : *data) {
		if (!entry.contains("sprite")) {//LATER SHOULD DECOUPLE OTHER DATA INTO ANOTHER JSON FOR CORRECTNESS. currently this json can contain other data
			continue;
		}

		const auto& spriteData = entry["sprite"];

		//kill it if we miss sprite information, otherwise we will eventually crash somewhere anyway
		if (!spriteData.contains("texture_id") || !spriteData.contains("texture_path") || !spriteData.contains("texture_source") || !spriteData.contains("texture_destination")) {
			throw std::logic_error("\nSprite data missing data\n");
		}
		//init data
		SpriteID textureID = static_cast<SpriteID>(spriteData["texture_id"]);

		SDL_FRect source{ spriteData["texture_source"][0], spriteData["texture_source"][1], spriteData["texture_source"][2], spriteData["texture_source"][3] };
		SDL_FRect destination{ spriteData["texture_destination"][0], spriteData["texture_destination"][1], spriteData["texture_destination"][2], spriteData["texture_destination"][3] };

		const std::string texturePath = spriteData["texture_path"];

		SDL_Texture* texture = IMG_LoadTexture(renderer, texturePath.c_str());

		//make sure the path actually existed, else kill it because we forgot to apply right name or at all
		if (!texture) {
			throw std::logic_error("\nFailed to load texture at path named: " + texturePath);
		}

		Sprite sprite(texture, &source, &destination);

		sprites.emplace(textureID, sprite);
	}
}
void EntityFactory::bootUpAnimations(const nlohmann::json* const data) {
	if (!data) {
		throw std::logic_error("\nJSON nullptr\n");
	}

	for (const auto& [name, obj]: data->items()) {
		if (!obj.contains("texture_id")) {//the texture MUST be defined with the primary key in both animation JSON and metadata JSOn
			throw std::logic_error("\nEntry without assigned ID (forgot to update?)\n");
		}

		SpriteID spriteID = static_cast<SpriteID>(obj["texture_id"]);
		for (const auto& [key, val] : obj.items()) {//assumes these items exist, otherwise something should throw
			if (key == "texture_id" || !val.is_object()) {
				continue;
			}
			AnimationClip clip;
			clip.x = val["begin_x"];
			clip.y = val["begin_y"];
			clip.w = val["width"];
			clip.h = val["height"];
			clip.frameCount = val["frame_count"];
			clip.frameDelay = val["frame_delay"];
			clip.isLooping = (bool)val["loops"];

			ClipID clipID = static_cast<ClipID>(val["clip_id"]);
			animationData[spriteID][clipID] = clip;
		}
	}
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
