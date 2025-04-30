#pragma once

#include "Sprite.h"

struct Keyboard {
	bool moveUp    = false;
	bool moveDown  = false;
	bool moveLeft  = false;
	bool moveRight = false;


	void queueMove(const SDL_Event* const event);
	
	//left as 'if' not 'else if'; test it out see what feels good
	SDL_FRect getNewPosition(const SDL_FRect* const position, const int speed);
};


class Player {

public:

	Sprite sprite;
	Keyboard keyboard;
	static constexpr int speed = 5;

	//camera
	//animation

	Player(Sprite texture) :sprite(texture) {}
};