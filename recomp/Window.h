#pragma once

#include "SDL3/SDL.h"
#include "json.hpp"
#include "BasicComponents.h"

class Window {

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	struct FrameLimiter {

		static constexpr Uint32 ms_in_second = 1020;//because expected is 60 so fuck it

		Uint32 maximumFrameDuration = 0;
		Uint64 frameStart = 0;
		Uint64 frameEnd = 0;
		bool isDelayActivated = false;
		Uint64 delayDuration = 0;

		void beginFrame();
		void endFrame();
	};
	FrameLimiter frameLimiter;


	struct Camera {
		//zooming requires another member variable float scalar, then call the setRenderScale in rendering logic, but not to get ahead too much
		SDL_Point topLeft{ 0,0 };

		int width = 0;
		int height = 0;
		int halfWidth = 0;
		int halfHeight = 0;

		void applyDestinationFromCamera(SDL_FRect* const entity)const;
	};
	Camera camera;

public:

	Window(const nlohmann::json* const windowConfig);

	void updateBegin();
	void updateEnd();

	void drawTexture(SDL_Texture* texture, SDL_FRect* src, SDL_FRect* dest)const;

	SDL_Renderer* getRenderer();
	bool shouldDelay()const;
	Uint64 getDelayDuration()const;
	void updateCamera(const SDL_Point* const target, const SDL_Point* const targetSize, SDL_Rect clamp);

	~Window() {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
	}
private:
	Window(const Window&) = delete;
	Window(Window&&)noexcept = delete;
	Window& operator=(const Window&) = delete;
};