#include "Texture_Manager.h"


SDL_Texture* TextureManager::GetTexture(const std::string& name) const
{
	auto it = textures.find(name);
	if (it != textures.end()) {
		return it->second;
	}

	throw "why you enter wrong/missing name baka?";
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
