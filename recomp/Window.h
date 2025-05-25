#pragma once

#include "SDL3/SDL.h"
#include "json.hpp"
#include "BasicComponents.h"

class Window {

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	class FrameLimiter {

		static constexpr Uint32 ms_in_second = 1000000;

		Uint32 FPS = 0;
		Uint32 maximumFrameDuration = 0;

		Uint32 frameX = 0;
		Uint32 frameY = 0;

		bool isDelayActivated = false;
		Uint32 delayDuration = 0;
	public:


		FrameLimiter() = default;
		FrameLimiter(const int fps) :FPS(fps), maximumFrameDuration(ms_in_second / FPS) {}

		void update() {
			frameY = frameX;
			frameX = SDL_GetTicks();
			Uint32 duration = frameX - frameY;

			if (duration < maximumFrameDuration) {
				delayDuration = maximumFrameDuration - duration;
				isDelayActivated = true;
			}
		}

		bool isDelay()const {
			return isDelayActivated;
		}
		Uint32 getDelayDuration()const {
			return delayDuration;
		}


		void frameBufferBegin();
		void frameBufferEnd();
	};
	FrameLimiter frameLimiter;


	class Camera {

		SDL_Point center{ 0,0 };
		SDL_Point topLeft{ 0,0 };
		int radiusWidth = 0;
		int radiusHeight = 0;

		//zooming requires another member variable float scalar, then call the setRenderScale in rendering logic, but not to get ahead too much
	public:
		Camera() = default;
		Camera(const int w, const int h):radiusWidth(w),radiusHeight(h) {}
		void clampTo(int x, int y, int w, int h);//duplicate function, kind of
		void applyDestinationFromCamera(SDL_FRect* const entity)const;
		void setFocusPoint(const SDL_Point* const pos, const SDL_Point* const size);
		void setTopLeft();
	};
	Camera camera;

public:

	Window(const nlohmann::json* const windowConfig);

	void updateBegin();
	void updateEnd();

	void drawTexture(TextureData* const sprite)const;

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