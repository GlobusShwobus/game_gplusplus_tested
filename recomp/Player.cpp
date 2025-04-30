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