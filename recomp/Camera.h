#pragma once
#include "SDL3/SDL.h"
namespace badEngine {

	class Camera2D {
		float offsetX = 0.0f;
		float offsetY = 0.0f;
		float scaleX = 1.0f;
		float scaleY = 1.0f;

		int screenW = 0;
		int screenH = 0;

	public:

		Camera2D(int screenW, int screenH) :screenW(screenW), screenH(screenH) {}

		void setScreenSize(int width, int height);
		void centerOn(float worldX, float worldY);
		void move(float dx, float dy);
		void zoom(float factor);
		void worldToScreen(float worldX, float worldY, float& screenX, float& screenY)const;
		void screenToWorld(float screenX, float screenY, float& worldX, float& worldY)const;

		float getScaleX() const;
		float getScaleY() const;
	};

	class Camera2DSDL :public Camera2D {
	
	public:
		Camera2DSDL(int width, int height):Camera2D(width, height) {}

		void centerOn(const SDL_FRect& rect);
		SDL_FRect worldToScreen(const SDL_FRect& rect)const;
	};

}