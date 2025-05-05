#pragma once
#include <chrono>
#include <thread>

#include "SDL3/SDL.h"
#include "json.hpp"


class RenderWindow {

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	const double frameDelay = 1000.0 / 60;
	Uint64 frameBegin = 0;
	Uint64 frameDuration = 0;

public:

	RenderWindow(const nlohmann::json* const config);


	void updateBegin() {
		SDL_RenderClear(renderer);
		frameBegin = SDL_GetTicks();
	}
	void updateEnd() {
		SDL_RenderPresent(renderer);

		frameDuration = SDL_GetTicks() - frameBegin;

		//weird fuckyness when mouse is moved in window, causes massive lag, sdl_evnet to blame? fuck knows
		if (frameDelay > frameDuration) {
			//SDL_Delay(frameDelay - frameDuration);
		}
	}

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