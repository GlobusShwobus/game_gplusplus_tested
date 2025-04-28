#pragma once

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "Entity.h"

class RenderWindow {

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

public:

	RenderWindow(const char* title, int width, int height);

	bool good()const;//can be depricated into a more global generic func that checks pointers
	void clear();
	void display();
	void render(const Sprite& sprite);

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