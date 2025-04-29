#pragma once

#include "Entity.h"
#include "Grid.h"
#include <array>

struct Keyboard {
	bool moveUp    = false;
	bool moveDown  = false;
	bool moveLeft  = false;
	bool moveRight = false;


	void queueMove(const SDL_Event* const event);
	void processMove(Grid& grid, SDL_FRect* position, const int speed);

private:

	void move(Grid& grid, SDL_FRect* current, const Direction direction, const int speed)const;

	std::array<SDL_FPoint, 4> getPoints(const SDL_FRect& position)const {//move this helper somewhere better
		std::array<SDL_FPoint, 4> corners = {
			SDL_FPoint{position.x, position.y}, //left top
			SDL_FPoint{position.x + position.w - 1, position.y},//right top
			SDL_FPoint{position.x, position.y + position.h - 1}, //left bottom
			SDL_FPoint{position.x + position.w - 1, position.y + position.h - 1}//right bottom
		};
		return corners;
	}
	SDL_FRect speedOffSet(const SDL_FRect* const original, const Direction direction, const int speed)const {//move this helper somewhere better

		SDL_FRect offSet = *original;

		switch (direction) {
		case Direction::up:	    offSet.y -= speed; break;
		case Direction::down:	offSet.y += speed; break;
		case Direction::left:	offSet.x -= speed; break;
		case Direction::right:	offSet.x += speed; break;
		}

		return offSet;
	}
};


class Player {

public:

	Sprite sprite;
	Keyboard keyboard;
	static constexpr int speed = 5;

	//movement
	//camera
	//animation


	Player(Sprite texture) :sprite(texture) {}

};