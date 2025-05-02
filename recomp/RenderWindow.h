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

	void clear();
	void display();
	SDL_Texture* loadNewTexture(const char* path);
	SDL_Renderer* getRenderer();
	void setLogicalRenderingSize(const int width, const int height);

	~RenderWindow() {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
	}
private:
	RenderWindow(const RenderWindow&) = delete;
	RenderWindow(RenderWindow&&)noexcept = delete;
	RenderWindow& operator=(const RenderWindow&) = delete;
};