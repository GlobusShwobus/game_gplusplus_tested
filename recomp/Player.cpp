#include "Player.h"

void Keyboard::queueMove(const SDL_Event* const event) {
	if (event->type == SDL_EVENT_KEY_DOWN) {
		switch (event->key.scancode) {
		case SDL_SCANCODE_W: moveUp = true; break;
		case SDL_SCANCODE_S: moveDown = true; break;
		case SDL_SCANCODE_A: moveLeft = true; break;
		case SDL_SCANCODE_D: moveRight = true; break;
		default:break;
		}
	}
}

SDL_FRect Keyboard::getNewPosition(const SDL_FRect* const position, const int speed) {
	SDL_FRect newPos = *position;
	if (moveUp) {
		newPos.y -= speed;
		moveUp = false;
	}
	if (moveDown) {
		newPos.y += speed;
		moveDown = false;
	}
	if (moveLeft) {
		newPos.x -= speed;
		moveLeft = false;
	}
	if (moveRight) {
		newPos.x += speed;
		moveRight = false;
	}
	return newPos;
}
void Camera::setFocus(const SDL_FRect* const playerPos) {
	//camera center at player's center
	center.x = playerPos->x + (playerPos->w / 2);
	center.y = playerPos->y + (playerPos->h / 2);
}
void Camera::setClamp(const SDL_FRect* const worldMap) {
	if (center.x - radiusWidth < 0) { center.x = radiusWidth; }//left edge
	if (center.y - radiusHeight < 0) { center.y = radiusHeight; }//top edge
	if (center.x + radiusWidth > worldMap->w) { center.x = worldMap->w - radiusWidth; }//right edge
	if (center.y + radiusHeight > worldMap->h) { center.y = worldMap->h - radiusHeight; }//bottom edge
}
const SDL_FPoint Camera::getCenter()const {
	return center;
}
SDL_FRect Camera::toCameraSpace(const SDL_FRect* const entity)const {
	SDL_FRect screenPos{ 0,0,0,0 };

	screenPos.x = entity->x - (center.x - radiusWidth);
	screenPos.y = entity->y - (center.y - radiusHeight);

	screenPos.w = entity->w;
	screenPos.h = entity->h;

	return screenPos;
}

