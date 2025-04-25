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
			move(grid, pos, Direction::up, moveUp);
		}
		else if (moveDown) {
			move(grid, pos, Direction::down, moveDown);
		}
		else if (moveLeft) {
			move(grid, pos, Direction::left, moveLeft);
		}
		else if (moveRight) {
			move(grid, pos, Direction::right, moveRight);
		}
	}

private:
	void move(Grid& grid, vec2f& pos, const Direction direction, bool& key) {

		const vec2f current = grid.convertToGrid(pos);
		const vec2f next = current + grid.getDirection(direction);

		if (!grid.isFilled(next)) {
			grid.setTile(current, false);
			grid.setTile(next, true);

			pos = grid.convertToPixel(next);
			
			printf("\nyes i moved");
		}
		key = false;
	}

};


class Player {

public:

	Sprite sprite;
	Keyboard keyboard;

	Player(Sprite texture) :sprite(texture) {}

};