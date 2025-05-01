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

struct Camera {

	SDL_FRect camera{ 0,0,0,0 };

	Camera(float renderedWidth, float renderedHeight) :camera{ 0,0,renderedWidth,renderedHeight } {}

	void update(const SDL_FRect* const playerPos, const SDL_FRect* const worldMap);
};


class Player {

public:

	Sprite sprite;
	Keyboard keyboard;
	Camera camera;

	static constexpr int speed = 5;

	//camera
	//animation

	Player(Sprite texture) :sprite(texture), camera(800.f, 600.f) {}
};