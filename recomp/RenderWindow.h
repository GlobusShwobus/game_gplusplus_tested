#pragma once

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "Sprite.h"
#include "json.hpp"

class RenderWindow {

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

public:

	RenderWindow(const nlohmann::json* const config);

	bool good()const;//can be depricated into a more global generic func that checks pointers
	void clear();
	void display();

	void renderMap(const Sprite& mapSprite, const SDL_FRect* const camera) {
		int screenWidth = 0;
		int scrrenHeight = 0;

		SDL_GetCurrentRenderOutputSize(renderer, &screenWidth, &scrrenHeight);

		SDL_FRect destination = { 0, 0, screenWidth, scrrenHeight };


		SDL_RenderTexture(renderer, mapSprite.texture, camera, &destination);
	}


	void renderBasic(const Sprite& sprite, const SDL_FRect& camera) {
		int screenWidth = 0;
		int screenHeight = 0;

		SDL_GetCurrentRenderOutputSize(renderer, &screenWidth, &screenHeight);


		/*
		spriteX-camX = RelX => pos relative to camera origin
		Relx/camW = Norm => what % of the cameras view has the item moved into
		Norm * screenW = scale the drawing area to be proportional
		(same logic for others)
		*/

		SDL_FRect screenDest;
		screenDest.x = ((sprite.destination.x - camera.x) / camera.w) * screenWidth;
		screenDest.y = ((sprite.destination.y - camera.y) / camera.h) * screenHeight;
		screenDest.w = (sprite.destination.w / camera.w) * screenWidth;
		screenDest.h = (sprite.destination.h / camera.h) * screenHeight;

		SDL_RenderTexture(renderer, sprite.texture, &sprite.source, &screenDest);
	}


	SDL_Renderer* getRenderer() {
		return renderer;
	}

	SDL_Texture* loadNewTexture(const char* path);

	~RenderWindow() {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
	}
private:
	RenderWindow(const RenderWindow&) = delete;
	RenderWindow(RenderWindow&&)noexcept = delete;
	RenderWindow& operator=(const RenderWindow&) = delete;
};