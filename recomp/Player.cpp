#include "Player.h"

void Movement::moveBegin(const SDL_Event* const event) {
	if (event->type == SDL_EVENT_KEY_DOWN) {
		switch (event->key.scancode) {
		case SDL_SCANCODE_W: movementStatus |= (MFLAG_IS_MOVING | MFLAG_KEY_W); break;
		case SDL_SCANCODE_S: movementStatus |= (MFLAG_IS_MOVING | MFLAG_KEY_S); break;
		case SDL_SCANCODE_A: movementStatus |= (MFLAG_IS_MOVING | MFLAG_KEY_A); break;
		case SDL_SCANCODE_D: movementStatus |= (MFLAG_IS_MOVING | MFLAG_KEY_D); break;
		default:break;
		}
	}
}
void Movement::moveEnd() {
	movementStatus &= ~(MFLAG_KEY_W | MFLAG_KEY_S | MFLAG_KEY_A | MFLAG_KEY_D | MFLAG_IS_MOVING);
}
int Movement::getMovementStatus()const {
	return movementStatus;
}
void Movement::setStatusFlags(const MFLAG flags) {
	movementStatus |= flags;
}
void Movement::removeStatusFlags(const MFLAG flags) {
	movementStatus &= ~flags;
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

