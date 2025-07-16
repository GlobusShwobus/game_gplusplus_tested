#include "Camera.h"

namespace badEngine {

	Vek2 Camera2D::worldToScreen(const Vek2& position) const
	{
		Vek2 p;
		worldToScreen(position.x, position.y, p.x, p.y);
		return p;
	}
	Vek2 Camera2D::worldToScreen(float x, float y) const
	{
		Vek2 p;
		worldToScreen(x, y, p.x, p.y);
		return p;
	}
	Rectangle Camera2D::worldToScreen(const Rectangle& rect)const
	{
		Vek2 p = worldToScreen(rect.x, rect.y);
		float w = rect.w * scaleX;
		float h = rect.h * scaleY;

		return { p.x, p.y, w,h };
	}
	Vek2 Camera2D::screenToWorld(const Vek2& position) const
	{
		Vek2 p;
		screenToWorld(position.x, position.y, p.x, p.y);
		return p;
	}
	Vek2 Camera2D::screenToWorld(float x, float y) const
	{
		Vek2 p;
		screenToWorld(x, y, p.x, p.y);
		return p;
	}
	Rectangle Camera2D::screenToWorld(const Rectangle& rect)const
	{
		Vek2 p = screenToWorld(rect.x, rect.y);
		float w = rect.w / scaleX;
		float h = rect.h / scaleY;

		return { p.x, p.y, w,h };
	}
}