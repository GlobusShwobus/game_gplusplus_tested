#pragma once

#include "Entity.h"
#include "Grid.h"

struct Keyboard {
	bool moveUp    = false;
	bool moveDown  = false;
	bool moveLeft  = false;
	bool moveRight = false;


	void queueMove(const SDL_Event* const event);
	void processMove(Grid& grid, vec2f& pos, const int speed);

private:

	void move(Grid& grid, vec2f& current, const Direction direction, const int speed)const;

	//yeet this function in misc pile
	vec2f speedOffset(const Direction direction, const float speed)const {
		vec2f speedOffset = { 0,0 };
		switch (direction) {
		case Direction::up:	    speedOffset.y -= speed; break;
		case Direction::down:	speedOffset.y += speed; break;
		case Direction::left:	speedOffset.x -= speed; break;
		case Direction::right:	speedOffset.x += speed; break;
		}
		return speedOffset;
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