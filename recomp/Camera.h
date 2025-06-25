#pragma once
#include "SDL3/SDL.h"

namespace badEngine {

	class ScreenSpace {

		float offsetX = 0.0f;
		float offsetY = 0.0f;

		float panX = 0.0f;
		float panY = 0.0f;

		float scaleX = 0.0f;
		float scaleY = 0.0f;

	public:

		void cWorldToScreen(float worldX, float worldY, float& screenX, float& screenY)const
		{
			screenX = (worldX - offsetX) * scaleX;
			screenY = (worldY - offsetY) * scaleY;
		}
		void cScreenToWorld(float screenX, float screenY, float& worldX, float& worldY)const
		{
			worldX = screenX / scaleX + offsetX;
			worldY = screenY / scaleY + offsetY;
		}
	};

	class Camera : public ScreenSpace {

	public:
		SDL_FPoint worldToScreen(const SDL_FPoint& inWorldPos)const
		{
			SDL_FPoint iwp = inWorldPos;
			cWorldToScreen(inWorldPos.x, inWorldPos.y, iwp.x, iwp.y);
			return iwp;
		}
		SDL_FPoint screenToWorld(const SDL_FPoint& inScreenPos)const
		{
			SDL_FPoint isp = inScreenPos;
			cScreenToWorld(inScreenPos.x, inScreenPos.y, isp.x, isp.y);
			return isp;
		}
	};

}