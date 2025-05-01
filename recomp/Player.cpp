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
void Camera::update(const SDL_FRect* const playerPos, const SDL_FRect* const worldMap) {
	//camera top left = player top left - camera width in half + player width in half (same for height)
	camera.x = playerPos->x - (camera.w / 2) + (playerPos->w / 2);
	camera.y = playerPos->y - (camera.h / 2) + (playerPos->h / 2);

	//stop the camera at edges, (at edges don't follow player) 
	if (camera.x < 0) { camera.x = 0; }
	if (camera.y < 0) { camera.y = 0; }
	if (camera.x + camera.w > worldMap->w) { camera.x = worldMap->w - camera.w; }
	if (camera.y + camera.h > worldMap->h) { camera.y = worldMap->h - camera.h; }
}