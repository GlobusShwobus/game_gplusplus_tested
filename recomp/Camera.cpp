#include "Camera.h"

namespace badEngine {

	Point Camera2D::worldToScreen(const Point& position) const
	{
		Point p;
		worldToScreen(position.x, position.y, p.x, p.y);
		return p;
	}
	Point Camera2D::worldToScreen(float x, float y) const
	{
		Point p;
		worldToScreen(x, y, p.x, p.y);
		return p;
	}
	Rectangle Camera2D::worldToScreen(const Rectangle& rect)const
	{
		Point p = worldToScreen(rect.x, rect.y);
		float w = rect.w * scaleX;
		float h = rect.h * scaleY;

		return { p.x, p.y, w,h };
	}
	Point Camera2D::screenToWorld(const Point& position) const
	{
		Point p;
		screenToWorld(position.x, position.y, p.x, p.y);
		return p;
	}
	Point Camera2D::screenToWorld(float x, float y) const
	{
		Point p;
		screenToWorld(x, y, p.x, p.y);
		return p;
	}
	Rectangle Camera2D::screenToWorld(const Rectangle& rect)const
	{
		Point p = screenToWorld(rect.x, rect.y);
		float w = rect.w / scaleX;
		float h = rect.h / scaleY;

		return { p.x, p.y, w,h };
	}
}