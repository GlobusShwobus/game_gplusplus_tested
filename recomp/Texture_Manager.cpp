#include "Texture_Manager.h"

void TextureManager::InitializeTexture(const std::string& name, SDL_Texture* texture)
{
	textures.emplace(name, texture);
	/*
	for (const std::filesystem::path& path : texture_path_list) {
		SDL_Texture* texture = IMG_LoadTexture(renderer, path.string().c_str());

		if (!texture) {
			printf("Failed to load texture >>> %s", path.string().c_str());
			continue;
		}

		textures.emplace(path.stem().string(), texture);
	}
	*/
}

SDL_Texture* TextureManager::GetTexture(const std::string& name) const
{
	auto it = textures.find(name);
	if (it != textures.end()) {
		return it->second;
	}

	return nullptr;
}

TextureManager::~TextureManager()
{
	for (auto& [name, texture] : textures) {
		if (texture) {
			SDL_DestroyTexture(texture);
		}
	}
	textures.clear();
}
