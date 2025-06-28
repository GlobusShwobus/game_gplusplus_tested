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

		void setScreenSize(int width, int height) {
			screenW = width;
			screenH = height;
		}
		void centerOn(float worldX, float worldY) {
			offsetX = worldX - (screenW / 2.0f) / scaleX;
			offsetY = worldY - (screenH / 2.0f) / scaleY;
		}
		void move(float dx, float dy) {
			offsetX += dx / scaleX;
			offsetY += dy / scaleY;
		}
		void zoom(float factor) {
			scaleX *= factor;
			scaleY *= factor;
		}
		void worldToScreen(float worldX, float worldY, float& screenX, float& screenY) const {
			screenX = (worldX - offsetX) * scaleX;
			screenY = (worldY - offsetY) * scaleY;
		}
		void screenToWorld(float screenX, float screenY, float& worldX, float& worldY) const {
			worldX = screenX / scaleX + offsetX;
			worldY = screenY / scaleY + offsetY;
		}

		float getScaleX() const { return scaleX; }
		float getScaleY() const { return scaleY; }
	};

	class Camera2DSDL :public Camera2D {
	
	public:
		Camera2DSDL(int width, int height):Camera2D(width, height) {}

		void centerOn(const SDL_FRect& rect) {
			Camera2D::centerOn(rect.x + (rect.w * 0.5f), rect.y + (rect.h * 0.5f));
		}
		SDL_FRect worldToScreen(const SDL_FRect& rect) {
			SDL_FRect adjusted = rect;
			Camera2D::worldToScreen(rect.x, rect.y, adjusted.x, adjusted.y);
			adjusted.w *= getScaleX();
			adjusted.h *= getScaleY();
			return adjusted;
		}
	};

}