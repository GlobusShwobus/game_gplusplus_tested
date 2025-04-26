#pragma once

#include "Entity.h"
#include "Grid.h"

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
	void processMove(Grid& grid, vec2f& pos) {
				
		if (moveUp) {
			move(grid, pos, Direction::up);
			moveUp = false;
		}
		else if (moveDown) {
			move(grid, pos, Direction::down);
			moveDown = false;
		}
		else if (moveLeft) {
			move(grid, pos, Direction::left);
			moveLeft = false;
		}
		else if (moveRight) {
			move(grid, pos, Direction::right);
			moveRight = false;
		}
	}

private:

	void move(Grid& grid, vec2f& current, const Direction direction) {

		static const int speed = 5;//temporary 

		const vec2f newPosition = current + speedOffset(direction, speed);
		const bool isFilled = grid.isFilled(newPosition);
		const bool isSameTiles = grid.isSameTile(newPosition, current);


		if (isFilled) {
			return;
		}

		if (!isSameTiles) {
			grid.setTile(current, false);
			grid.setTile(newPosition, true);
		}

		current = newPosition;

		printf("\nyes i moved");
		
	}


	//i need to know the current most position and i need to know the next tile from current before i apply any logic
	//if the next from current is free, only then i should be allowed to apply the speed (meaning the amount of pixels to move)
	//however after applying speed i need to check if this new position would be the next tile or remain current tile
	//if it is next tile, i need to free up the current and set next
	//otherwise, since it is still the same tile, no need to reset tile states

	//dunno where to stick this atm
	vec2f speedOffset(const Direction direction, const int speed) {
		vec2f speedOffset;
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

	//add speed variable
	//move based on speed variable (in pixel measure)
	//create a secondary move function which checks next but moves based on speed not next, basically a simple second solution, 

	Player(Sprite texture) :sprite(texture) {}

};