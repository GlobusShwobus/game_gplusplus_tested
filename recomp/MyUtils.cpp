#include "MyUtils.h"

namespace MyUtils {
	std::array<SDL_FPoint, 4> getCorners(const SDL_FRect& position) {//move this helper somewhere better
		std::array<SDL_FPoint, 4> corners = {
			SDL_FPoint{position.x, position.y}, //left top
			SDL_FPoint{position.x + position.w - 1, position.y},//right top
			SDL_FPoint{position.x, position.y + position.h - 1}, //left bottom
			SDL_FPoint{position.x + position.w - 1, position.y + position.h - 1}//right bottom
		};
		return corners;
	}

	SDL_FRect getNewPosition(const SDL_FRect* const position, const MovementStatus movementStatus, const float speed) {
		SDL_FRect newPos = *position;

		const float diagonalSpeed = speed * 0.7071f;// if diagonal speed is faster by sqrt2, then adjust by 1/sqrt2=0.7071

		switch (movementStatus) {
		case MovementStatus::MOVE_UP:
			newPos.y -= speed;
			break;
		case MovementStatus::MOVE_DOWN:
			newPos.y += speed;
			break;
		case MovementStatus::MOVE_LEFT:
			newPos.x -= speed;
			break;
		case MovementStatus::MOVE_RIGHT:
			newPos.x += speed;
			break;
		case MovementStatus::MOVE_UP_LEFT:
			newPos.y -= diagonalSpeed;
			newPos.x -= diagonalSpeed;
			break;
		case MovementStatus::MOVE_UP_RIGHT:
			newPos.y -= diagonalSpeed;
			newPos.x += diagonalSpeed;
			break;
		case MovementStatus::MOVE_DOWN_LEFT:
			newPos.y += diagonalSpeed;
			newPos.x -= diagonalSpeed;
			break;
		case MovementStatus::MOVE_DOWN_RIGHT:
			newPos.y += diagonalSpeed;
			newPos.x += diagonalSpeed;
			break;
		default:
			break;
		}
		return newPos;
	}

	void updatePosition(Grid& grid, const SDL_FRect& updatedLocation, SDL_FRect* const previousPosition) {

		auto corners = getCorners(updatedLocation);

		for (const auto& point : corners) {
			if (!grid.isValidTile(point) || !grid.getTile(point).doesContain(TFLAG_WALKABLE)) {
				return;
			}
		}
		*previousPosition = updatedLocation;
	}

	nlohmann::json* initJSON(const char* path) {
		//look into assurances that the path is a json so we catch exception early
		std::ifstream in(path);
		nlohmann::json* json = nullptr;
		if (in.good()) {
			json = new nlohmann::json();
			in >> *json;
		}
		in.close();

		return json;
	}




	ClipID getClipBasedOnMovement(const Movement movementData) {

		MovementStatus moving = movementData.getCurrentMove();
		MovementStatus ifIdle = movementData.getLastMove();

		if (moving == MovementStatus::MOVE_UP) {
			return ClipID::walk_up;
		}
		else if (moving == MovementStatus::MOVE_DOWN) {
			return ClipID::walk_down;
		}
		else if (moving == MovementStatus::MOVE_LEFT || moving == MovementStatus::MOVE_UP_LEFT || moving == MovementStatus::MOVE_DOWN_LEFT) {
			return ClipID::walk_left;
		}
		else if (moving == MovementStatus::MOVE_RIGHT || moving == MovementStatus::MOVE_UP_RIGHT || moving == MovementStatus::MOVE_DOWN_RIGHT) {
			return ClipID::walk_right;
		}
		//if none of the above is satisfied
		if (ifIdle == MovementStatus::MOVE_UP) {
			return ClipID::idle_up;
		}
		else if (ifIdle == MovementStatus::MOVE_DOWN) {
			return ClipID::idle_down;
		}
		else if (ifIdle == MovementStatus::MOVE_LEFT || ifIdle == MovementStatus::MOVE_UP_LEFT || ifIdle == MovementStatus::MOVE_DOWN_LEFT) {
			return ClipID::idle_left;
		}
		else if (ifIdle == MovementStatus::MOVE_RIGHT || ifIdle == MovementStatus::MOVE_UP_RIGHT || ifIdle == MovementStatus::MOVE_DOWN_RIGHT) {
			return ClipID::idle_right;
		}

		return ClipID::none;
	}
}