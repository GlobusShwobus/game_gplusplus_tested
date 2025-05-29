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
				state.setDirection(NPCState::Direction::up);
			}
			else if (a) {
				state.setDirection(NPCState::Direction::left);
			}
			else if (s) {
				state.setDirection(NPCState::Direction::down);
			}
			else if (d) {
				state.setDirection(NPCState::Direction::right);
			}

		}
		else {
			state.setState(NPCState::State::idle);
		}
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
	AnimID getReelOnState(const NPCState& state) {
		const auto st = state.getState();
		const auto fc = state.getDirection();

		AnimID id = 0;

		if (st == NPCState::State::moving) {
			switch (fc) {
			case NPCState::Direction::up:	 id = AnimID_WALK_UP; break;
			case NPCState::Direction::down:  id = AnimID_WALK_DOWN; break;
			case NPCState::Direction::left:  id = AnimID_WALK_LEFT; break;
			case NPCState::Direction::right: id = AnimID_WALK_RIGHT; break;
			}
		}
		else if (st == NPCState::State::idle) {
			switch (fc) {
			case NPCState::Direction::up:	 id = AnimID_IDLE_UP; break;
			case NPCState::Direction::down:  id = AnimID_IDLE_DOWN; break;
			case NPCState::Direction::left:  id = AnimID_IDLE_LEFT; break;
			case NPCState::Direction::right: id = AnimID_IDLE_RIGHT; break;
			}
		}
		return id;
	}
	void moveScriptBasic(Transform& transform, NPCState& state, const float moveSpeed) {
		if (state.getState() == NPCState::State::idle) {
			return;
		}
		SDL_Point newVel = velocityOnDir(state.getDirection(), moveSpeed);
		transform.setVelocity(newVel);
		transform.setPosFromCurrentVel();
		transform.resetVelocity();
	}
	SDL_Point velocityOnDir(NPCState::Direction dir, const float moveSpeed) {
		SDL_Point vel{ 0,0 };

		switch (dir) {
		case NPCState::Direction::up:    vel.y -= (int)moveSpeed; break;
		case NPCState::Direction::down:  vel.y += (int)moveSpeed; break;
		case NPCState::Direction::left:  vel.x -= (int)moveSpeed; break;
		case NPCState::Direction::right: vel.x += (int)moveSpeed; break;
		default://no change but other dirs should not exist (maybe diagonals later)
			break;
		}
		return vel;
	}

}
