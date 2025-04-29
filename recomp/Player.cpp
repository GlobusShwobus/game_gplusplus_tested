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

void Keyboard::processMove(Grid& grid, SDL_FRect* position, const int speed) {

	if      (moveUp)    {      move(grid, position, Direction::up,    speed);      moveUp    = false; }
	else if (moveDown)  {      move(grid, position, Direction::down,  speed);      moveDown  = false; }
	else if (moveLeft)  {      move(grid, position, Direction::left,  speed);      moveLeft  = false; }
	else if (moveRight) {      move(grid, position, Direction::right, speed);      moveRight = false; }
}

void Keyboard::move(Grid& grid, SDL_FRect* current, const Direction direction, const int speed)const {

	const SDL_FRect newPosition = speedOffSet(current, direction, speed);
	auto corners = getPoints(newPosition);

	for (const auto& point : corners) {
		if (!grid.isValidTile(point) || !grid.getTile(point).doesContain(TFLAG_WALKABLE)) {
			return;
		}
	}

	*current = newPosition;
}