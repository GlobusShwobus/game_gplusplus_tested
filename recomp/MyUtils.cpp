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
	void WASD_state(NPCState& state) {
		const auto* keystate = SDL_GetKeyboardState(nullptr);

		bool w = keystate[SDL_SCANCODE_W];
		bool a = keystate[SDL_SCANCODE_A];
		bool s = keystate[SDL_SCANCODE_S];
		bool d = keystate[SDL_SCANCODE_D];


		if (w || a || s || d) {
			state.setState(NPCState::State::moving);
			if (w) {
				state.setFacing(NPCState::Facing::up);
			}
			if (a) {
				state.setFacing(NPCState::Facing::left);
			}
			if (s) {
				state.setFacing(NPCState::Facing::down);
			}
			if (d) {
				state.setFacing(NPCState::Facing::right);
			}

		}
		else {
			state.setState(NPCState::State::idle);
		}
	}

	void doMovement(Grid& grid, SDL_FRect* const currentPosition, const NPCState& state, const float speed) {

		if (state.getState() == NPCState::State::idle) {
			return;//no move so skip
		}

		SDL_FRect newPos = getNewPosition(currentPosition, state.getFacing(), speed);


		auto corners = getCorners(newPos);

		for (const auto& point : corners) {//if there is any collision with something which shouldn't allow pass through, skip move
			if (!grid.isValidTile(point) || !grid.getTile(point).doesContain(TFLAG_WALKABLE)) {
				return;
			}
		}
		*currentPosition = newPos;
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


	SDL_FRect getNewPosition(const SDL_FRect* const current, const NPCState::Facing& facing, const float speed) {
		SDL_FRect newPos = *current;

		//no diagonals currently anymore
		//const float diagonalSpeed = speed * 0.7071f;// if diagonal speed is faster by sqrt2, then adjust by 1/sqrt2=0.7071

		switch (facing) {
		case NPCState::Facing::up:    newPos.y -= speed; break;
		case NPCState::Facing::down:  newPos.y += speed; break;
		case NPCState::Facing::left:  newPos.x -= speed; break;
		case NPCState::Facing::right: newPos.x += speed; break;
		default://no change
			break;
		}
		return newPos;
	}
	AnimID getReelOnState(const NPCState& state) {
		const auto st = state.getState();
		const auto fc = state.getFacing();

		AnimID id = 0;

		if (st == NPCState::State::moving) {
			switch (fc) {
			case NPCState::Facing::up:	  id = AnimID_WALK_UP; break;
			case NPCState::Facing::down:  id = AnimID_WALK_DOWN; break;
			case NPCState::Facing::left:  id = AnimID_WALK_LEFT; break;
			case NPCState::Facing::right: id = AnimID_WALK_RIGHT; break;
			}
		}
		else if (st == NPCState::State::idle) {
			switch (fc) {
			case NPCState::Facing::up:	  id = AnimID_IDLE_UP; break;
			case NPCState::Facing::down:  id = AnimID_IDLE_DOWN; break;
			case NPCState::Facing::left:  id = AnimID_IDLE_LEFT; break;
			case NPCState::Facing::right: id = AnimID_IDLE_RIGHT; break;
			}
		}
		return id;
	}
}