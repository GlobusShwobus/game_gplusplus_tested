#pragma once

#include <map>

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "json.hpp"
#include "Player.h"

class TextureManager {

	std::map<SpriteID, Sprite> sprites;
	std::map<SpriteID, std::map<ClipID, AnimationClip>> animationData;

public:

	TextureManager() = default;

	void bootUpSprites(SDL_Renderer* renderer, const nlohmann::json* const data);
	void bootUpAnimations(const nlohmann::json* const data);

	//makes a copy
	Sprite createSprite(const SpriteID spriteID);
	~TextureManager();
public:
	TextureManager(const TextureManager&) = delete;
	TextureManager(TextureManager&&)noexcept = delete;
	TextureManager& operator=(const TextureManager&) = delete;
};

