#pragma once

#include "Rectangle.h"
#include "Point.h"

namespace badEngine {
	class Camera2D {

		float offsetX = 0.0f;
		float offsetY = 0.0f;
		float scaleX = 1.0f;
		float scaleY = 1.0f;

		int screenW = 0;
		int screenH = 0;

	public:

		Camera2D(int screenWidth, int screenHeight) :screenW(screenWidth), screenH(screenHeight) {}


		inline void updateScreenSize(int width, int height)
		{
			screenW = width;
			screenH = height;
		}
		inline void updateScreenSize(const Point& dimensions)
		{
			updateScreenSize(dimensions.x, dimensions.y);
		}

		inline void focusPoint(float x, float y)
		{
			offsetX = x - (screenW * 0.5f) / scaleX;	//abs
			offsetY = y - (screenH * 0.5f) / scaleY;	//abs
		}
		inline void focusPoint(const Point& position)
		{
			focusPoint(position.x, position.y);
		}
		inline void focusPoint(float x, float y, float w, float h)
		{
			focusPoint(x + (w * 0.5f), y + (h * 0.5f));
		}
		inline void focusPoint(const Rectangle& rect)
		{
			focusPoint(rect.x + rect.halfWidth(), rect.y + rect.halfHeight());
		}

		inline void move(float deltaX, float deltaY)
		{
			offsetX += deltaX / scaleX;
			offsetY += deltaY / scaleY;
		}
		inline void move(const Point& delta)
		{
			move(delta.x, delta.y);
		}

		inline void zoom(float factor)
		{
			scaleX *= factor;
			scaleY *= factor;
		}

		inline void worldToScreen(float worldX, float worldY, float& screenX, float& screenY) const
		{
			screenX = (worldX - offsetX) * scaleX;
			screenY = (worldY - offsetY) * scaleY;
		}
		Point worldToScreen(const Point& position)const;
		Point worldToScreen(float x, float y)const;
		Rectangle worldToScreen(const Rectangle& rect)const;

		inline void screenToWorld(float screenX, float screenY, float& worldX, float& worldY) const
		{
			worldX = screenX / scaleX + offsetX;
			worldY = screenY / scaleY + offsetY;
		}
		Point screenToWorld(const Point& position) const;
		Point screenToWorld(float x, float y) const;
		Rectangle screenToWorld(const Rectangle& rect)const;

		inline float getScaleX() const { return scaleX; }
		inline float getScaleY() const { return scaleY; }
	};
}
