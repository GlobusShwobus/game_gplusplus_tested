#pragma once

#include "SDL3/SDL.h"
#include "vector"

class Sprite {

	SDL_Texture* texture = nullptr;//not owner
	SDL_FRect source = { 0,0,0,0 };
	SDL_FRect destination = { 0,0,0,0 };


	bool isAnimated = false;
	int frameWidth = 0;
	int frameHeight = 0;
public:

	Sprite(SDL_Texture* Texture) :texture(Texture) {
		//defaults
		source.w = texture->w;
		source.h = texture->h;

		destination.w = texture->w;
		destination.h = texture->h;
	}

	//if a texture is animated, but framewidth and height are not set, then it is not animated, duhh
	void initFrame(int frameW, int frameH) {
		frameWidth = frameW;
		frameHeight = frameH;
		isAnimated = true;//the only place it is set
	}

	SDL_Texture* getTexture() {
		return texture;
	}
	SDL_FRect* getSource() {
		return &source;
	}
	SDL_FRect* getDestination() {
		return &destination;
	}

};