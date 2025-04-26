#pragma once

#include "SDL3/SDL.h"
#include "File_Manager.h"

class TextureManager {

	std::map<std::string, SDL_Texture*> textures;

public:

	TextureManager() = default;

	void cacheTexture(const std::string& name, SDL_Texture* texture);
	SDL_Texture* GetTexture(const std::string& name)const;

	~TextureManager();
public:
	TextureManager(const TextureManager&) = delete;
	TextureManager(TextureManager&&)noexcept = delete;
	TextureManager& operator=(const TextureManager&) = delete;
};

