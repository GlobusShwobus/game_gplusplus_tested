#include "Player.h"

void Movement::moveBegin(const SDL_Event* const event) {

	if (event->type != SDL_EVENT_KEY_DOWN) {
		return;
	}

	const auto* keystate = SDL_GetKeyboardState(nullptr);

	bool w = keystate[SDL_SCANCODE_W];
	bool a = keystate[SDL_SCANCODE_A];
	bool s = keystate[SDL_SCANCODE_S];
	bool d = keystate[SDL_SCANCODE_D];

	if (w && a) {
		movementStatus = MS::MOVE_UP_LEFT;
		facingDirection = MS::FACING_UP;
	}
	else if (w && d) {
		movementStatus = MS::MOVE_UP_RIGHT;
		facingDirection = MS::FACING_UP;
	}
	else if (s && a) {
		movementStatus = MS::MOVE_DOWN_LEFT;
		facingDirection = MS::FACING_DOWN;
	}
	else if (s && d) {
		movementStatus = MS::MOVE_DOWN_RIGHT;
		facingDirection = MS::FACING_DOWN;
	}
	else if (w) {
		movementStatus = MS::MOVE_UP;
		facingDirection = MS::FACING_UP;
	}
	else if (s) {
		movementStatus = MS::MOVE_DOWN;
		facingDirection = MS::FACING_DOWN;
	}
	else if (a) {
		movementStatus = MS::MOVE_LEFT;
		facingDirection = MS::FACING_LEFT;
	}
	else if (d) {
		movementStatus = MS::MOVE_RIGHT;
		facingDirection = MS::FACING_RIGHT;
	}
	else {
		movementStatus = MS::MOVEMENT_STATUS_NULL;
	}	
}
void Movement::moveEnd() {
	movementStatus = MS::MOVEMENT_STATUS_NULL;
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

