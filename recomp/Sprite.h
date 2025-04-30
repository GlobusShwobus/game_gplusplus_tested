#pragma once

#include "SDL3/SDL.h"

struct Sprite {
	SDL_Texture* texture = nullptr;
	SDL_FRect source = { 0,0,32,32 };
	SDL_FRect destination = { 0,0,32,32 };

	Sprite(SDL_Texture* pTexture) :texture(pTexture) {}
	Sprite(SDL_Texture* pTexture, SDL_FRect src, SDL_FRect dest) :texture(pTexture), source(src), destination(dest) {}
};