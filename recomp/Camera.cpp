#include "Camera.h"

namespace badEngine {
	void Camera2D::setScreenSize(int width, int height)
	{
		screenW = width;
		screenH = height;
	}
	void Camera2D::centerOn(float worldX, float worldY)
	{
		offsetX = worldX - (screenW / 2.0f) / scaleX;
		offsetY = worldY - (screenH / 2.0f) / scaleY;
	}
	void Camera2D::move(float dx, float dy)
	{
		offsetX += dx / scaleX;
		offsetY += dy / scaleY;
	}
	void Camera2D::zoom(float factor)
	{
		scaleX *= factor;
		scaleY *= factor;
	}
	void Camera2D::worldToScreen(float worldX, float worldY, float& screenX, float& screenY) const
	{
		screenX = (worldX - offsetX) * scaleX;
		screenY = (worldY - offsetY) * scaleY;
	}
	void Camera2D::screenToWorld(float screenX, float screenY, float& worldX, float& worldY) const
	{
		worldX = screenX / scaleX + offsetX;
		worldY = screenY / scaleY + offsetY;
	}

	float Camera2D::getScaleX() const { return scaleX; }
	float Camera2D::getScaleY() const { return scaleY; }

	void Camera2DSDL::centerOn(const SDL_FRect& rect)
	{
		Camera2D::centerOn(rect.x + (rect.w * 0.5f), rect.y + (rect.h * 0.5f));
	}
	SDL_FRect Camera2DSDL::worldToScreen(const SDL_FRect& rect)const
	{
		SDL_FRect adjusted = rect;
		Camera2D::worldToScreen(rect.x, rect.y, adjusted.x, adjusted.y);
		adjusted.w *= getScaleX();
		adjusted.h *= getScaleY();
		return adjusted;
	}
}