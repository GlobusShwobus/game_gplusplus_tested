#include "Texture_Manager.h"

void TextureManager::cacheTexture(const std::string& name, SDL_Texture* texture)
{
	textures.emplace(name, texture);
}

SDL_Texture* TextureManager::GetTexture(const std::string& name) const
{
	auto it = textures.find(name);
	if (it != textures.end()) {
		return it->second;
	}

	return nullptr;//MAKE IT RETURN SOME DEFAULT ERROR TEXTURE
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
