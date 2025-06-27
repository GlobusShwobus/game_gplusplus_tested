#pragma once

namespace badEngine {

	class Camera2D {
		float offsetX = 0.0f;
		float offsetY = 0.0f;
		float scaleX = 1.0f;
		float scaleY = 1.0f;

		int screenW = 0;
		int screenH = 0;

	public:

		Camera2D(int screenW, int screenH) :screenW(screenW), screenH(screenH) {
			centerOn(0.0f, 0.0f);
		}

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

}