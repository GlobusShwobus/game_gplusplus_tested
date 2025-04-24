#pragma once

#include "Entity.h"

struct Keyboard {
	bool moveUp    = false;
	bool moveDown  = false;
	bool moveLeft  = false;
	bool moveRight = false;


	void queueMove(const SDL_Event* const event) {
        if (event->type == SDL_EVENT_KEY_DOWN) {
            switch (event->key.scancode) {
            case SDL_SCANCODE_W: moveUp    = true; break;
            case SDL_SCANCODE_S: moveDown  = true; break;
            case SDL_SCANCODE_A: moveLeft  = true; break;
            case SDL_SCANCODE_D: moveRight = true; break;
            default:break;
            }
        }
	}
	void processMove() {

	}


};


class Player {

public:

	Sprite sprite;
	Keyboard keyboard;

	Player(Sprite texture) :sprite(texture) {}

};