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
	std::pair<NPCState::State, NPCState::Facing> getWASDState() {
		const auto* keystate = SDL_GetKeyboardState(nullptr);

		bool w = keystate[SDL_SCANCODE_W];
		bool a = keystate[SDL_SCANCODE_A];
		bool s = keystate[SDL_SCANCODE_S];
		bool d = keystate[SDL_SCANCODE_D];

		NPCState::State state = NPCState::State::noChange;
		NPCState::Facing facing = NPCState::Facing::noChange;

		if (w || a || s || d) {
			state = NPCState::State::walking;

			if (w) {
				facing = NPCState::Facing::up;
			}
			if (a) {
				facing = NPCState::Facing::left;
			}
			if (s) {
				facing = NPCState::Facing::down;
			}
			if (d) {
				facing = NPCState::Facing::right;
			}

		}
		return { state, facing };
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


	SDL_FRect getNewPosition(const SDL_FRect& position, const NPCState& state, const float speed) {
		SDL_FRect newPos = position;

		//const float diagonalSpeed = speed * 0.7071f;// if diagonal speed is faster by sqrt2, then adjust by 1/sqrt2=0.7071

		if(state.getState() == NPCState::State::walking) {
			switch (state.getFacing()) {
			case NPCState::Facing::up:    newPos.y -= speed; break;
			case NPCState::Facing::down:  newPos.y += speed; break;
			case NPCState::Facing::left:  newPos.x -= speed; break;
			case NPCState::Facing::right: newPos.x += speed; break;
			default://no change
				break;
			}
		}
		return newPos;
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