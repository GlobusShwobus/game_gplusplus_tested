#pragma once

#include "SDL3/SDL.h"
#include "json.hpp"
#include "BasicComponents.h"

class Window {

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	class FrameLimiter {

		int FPS = 0;
		const double frameDelay = 1000.0 / FPS;
		Uint64 frameBegin = 0;
		Uint64 frameDuration = 0;

	public:

		FrameLimiter(int fps) :FPS(fps) {}
		
		void frameBufferBegin() {
			frameBegin = SDL_GetTicks();
		}
		void frameBufferEnd() {
			frameDuration = SDL_GetTicks() - frameBegin;
			if (frameDelay > frameDuration) {
				SDL_Delay(frameDelay - frameDuration);
			}
		}
	};
	FrameLimiter* frameLimiter = nullptr;


	class Camera {

		SDL_FPoint center{ 0,0 };
		int radiusWidth = 0;
		int radiusHeight = 0;

		//zooming requires another member variable float scalar, then call the setRenderScale in rendering logic, but not to get ahead too much
	
		void setClamp(const int windowW, const int windowH);
	public:

		Camera(const int diameterWidth, const int diameterHeight) :radiusWidth(diameterWidth / 2), radiusHeight(diameterHeight / 2) {}

		void setFocus(const SDL_FRect* const playerPos);
		const SDL_FPoint getCenter()const;
		SDL_FRect toCameraSpace(const SDL_FRect* const entity)const;
	};
	Camera* camera = nullptr;

public:

	Window(const nlohmann::json* const windowConfig);

	void updateBegin();
	void updateEnd();

	void drawSprite(Sprite* sprite)const;

	SDL_Renderer* getRenderer();

	~Window() {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		delete camera;
		delete frameLimiter;
	}
private:
	Window(const Window&) = delete;
	Window(Window&&)noexcept = delete;
	Window& operator=(const Window&) = delete;
};