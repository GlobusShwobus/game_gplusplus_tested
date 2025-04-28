#include "Player.h"

void Keyboard::queueMove(const SDL_Event* const event) {
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

void Keyboard::processMove(Grid& grid, vec2f& pos, const int speed) {

	if      (moveUp)    {      move(grid, pos, Direction::up,    speed);      moveUp    = false; }
	else if (moveDown)  {      move(grid, pos, Direction::down,  speed);      moveDown  = false; }
	else if (moveLeft)  {      move(grid, pos, Direction::left,  speed);      moveLeft  = false; }
	else if (moveRight) {      move(grid, pos, Direction::right, speed);      moveRight = false; }
}

void Keyboard::move(Grid& grid, vec2f& current, const Direction direction, const int speed)const {

	const vec2f newPosition = current + speedOffset(direction, speed);
	const bool isValid = grid.isValid(newPosition);


	if (!isValid) {
		return;
	}
	
	auto& tile = grid.getTile(newPosition);
	const bool isWalkable = tile.isWalkable();

	if (!isWalkable) {
		return;
	}

	current = newPosition;
}