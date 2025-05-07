#pragma once

#include <fstream>
#include <filesystem>
#include <map>

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "json.hpp"
#include "Player.h"

//add better throw exceptions

class TextureManager {

	std::map<std::string, SDL_Texture*> textures;

public:

	TextureManager(SDL_Renderer* renderer, const char* folderPath) {
		//get the paths for textures
		std::vector<std::filesystem::path> texturePaths;

		for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
			if (entry.path().extension() == ".png") {
				texturePaths.push_back(entry);
			}
		}

		//sanity check
		if (texturePaths.empty()) {
			throw "\ntextures missing for some reason, folder deleted or wrong path\n";
		}

		//cache textures
		for (const auto& path : texturePaths) {

			SDL_Texture* texture = IMG_LoadTexture(renderer, path.string().c_str());
			textures.emplace(path.stem().string(), texture);
		}
	}

	Sprite createSprite(const nlohmann::json* const data) {
		if (!data->contains("texture_name")) {//if not texture then wtf are we even doing
			throw "\nJSON does not contain texture name or wrong name assignment wrong in JSON: must be 'texture_name'\n";
		}

		const std::string key = (*data)["texture_name"];//joink the key

		if (textures.find(key) == textures.end()) {//this might be not great, idk
			throw "\nThis texture is not cached";
		}

		Sprite sprite(textures[key]);

		return sprite;
	}

	SDL_Texture* GetTexture(const std::string& name)const;

	~TextureManager();
public:
	TextureManager(const TextureManager&) = delete;
	TextureManager(TextureManager&&)noexcept = delete;
	TextureManager& operator=(const TextureManager&) = delete;
};

