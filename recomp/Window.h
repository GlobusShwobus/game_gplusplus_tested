#pragma once

#include "SDL3/SDL.h"
#include "json.hpp"
#include "BasicComponents.h"

class Window {

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	struct FrameLimiter {

		static constexpr Uint32 ms_in_second = 1000;

		Uint32 maximumFrameDuration = 0;
		Uint32 frameStart = 0;
		Uint32 frameEnd = 0;
		bool isDelayActivated = false;
		Uint32 delayDuration = 0;

		void beginFrame();
		void endFrame();
	};
	FrameLimiter frameLimiter;


	struct Camera {

		SDL_Point topLeft{ 0,0 };

		int width = 0;
		int height = 0;
		int halfWidth = 0;
		int halfHeight = 0;

		//zooming requires another member variable float scalar, then call the setRenderScale in rendering logic, but not to get ahead too much
		void applyDestinationFromCamera(SDL_FRect* const entity)const;
	};
	Camera camera;

public:

	Window(const nlohmann::json* const windowConfig);

	void updateBegin();
	void updateEnd();

	void drawTexture(TextureData* const sprite)const;

	SDL_Renderer* getRenderer();
	bool shouldDelay()const;
	Uint32 getDelayDuration()const;
	void updateCamera(const SDL_Point* const target, const SDL_Point* const targetSize, SDL_Rect clamp) {

		camera.topLeft.x = (target->x + (targetSize->x / 2)) - camera.halfWidth;
		camera.topLeft.y = (target->y + (targetSize->y / 2)) - camera.halfHeight;


		if (camera.topLeft.x < clamp.x) {
			camera.topLeft.x = clamp.x;
		}
		if (camera.topLeft.y < clamp.y) {
			camera.topLeft.y = clamp.y;
		}
		if (camera.topLeft.x + camera.width > clamp.w) {
			camera.topLeft.x = clamp.w - camera.width;
		}
		if (camera.topLeft.y + camera.height > clamp.h) {
			camera.topLeft.y = clamp.h - camera.height;
		}
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