#pragma once

#include "SDL3/SDL.h"
#include "json.hpp"
#include "BasicComponents.h"

class Window {

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	class FrameLimiter {

		int FPS = 0;
		double frameDelay = 0;
		Uint64 frameBegin = 0;
		Uint64 frameDuration = 0;

	public:
		FrameLimiter() = default;

		void init(const int fps);
		void frameBufferBegin();
		void frameBufferEnd();
	};
	FrameLimiter frameLimiter;


	class Camera {

		SDL_FPoint center{ 0,0 };
		int radiusWidth = 0;
		int radiusHeight = 0;

		//zooming requires another member variable float scalar, then call the setRenderScale in rendering logic, but not to get ahead too much
	public:

		void init(const int width, const int height);
		void clampTo(const SDL_FRect* const rect);
		SDL_FRect toCameraSpace(const SDL_FRect* const entity)const;
		void setFocusPoint(const SDL_FRect* const rect);
	};
	Camera camera;

public:

	Window(const nlohmann::json* const windowConfig);

	void updateBegin();
	void updateEnd();

	void drawSprite(Sprite* sprite)const;

	SDL_Renderer* getRenderer();
	Camera* const getCamera() {
		return &camera;
	}

	~Window() {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
	}
private:
	Window(const Window&) = delete;
	Window(Window&&)noexcept = delete;
	Window& operator=(const Window&) = delete;
};