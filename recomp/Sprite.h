#pragma once

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "vec2.h"

struct Sprite {
	SDL_Texture* texture = nullptr;
	SDL_FRect source = { 0,0,32,32 };
	SDL_FRect destination = { 0,0,32,32 };


	void setDestination(const vec2f& position) {
		destination.x = position.x;
		destination.y = position.y;
	}


	Sprite(SDL_Texture* pTexture) :texture(pTexture) {}
	Sprite(SDL_Texture* pTexture, SDL_FRect src, SDL_FRect dest) :texture(pTexture), source(src), destination(dest) {}
};