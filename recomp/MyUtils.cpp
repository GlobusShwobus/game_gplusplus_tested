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
	void WASD_state(EntityState& state) {
		const auto* keystate = SDL_GetKeyboardState(nullptr);

		bool w = keystate[SDL_SCANCODE_W];
		bool a = keystate[SDL_SCANCODE_A];
		bool s = keystate[SDL_SCANCODE_S];
		bool d = keystate[SDL_SCANCODE_D];


		if (w || a || s || d) {
			state.changeAction(EntityAction::moving);
			if (w) {
				state.changeDirection(EntityDirection::up);
			}
			else if (a) {
				state.changeDirection(EntityDirection::left);
			}
			else if (s) {
				state.changeDirection(EntityDirection::down);
			}
			else if (d) {
				state.changeDirection(EntityDirection::right);
			}

		}
		else {
			state.changeAction(EntityAction::idle);
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
	AnimID getReelOnState(EntityAction action, EntityDirection direction) {
		AnimID id = 0;

		if (action == EntityAction::moving) {
			switch (direction) {
			case EntityDirection::up:	 id = AnimID_WALK_UP; break;
			case EntityDirection::down:  id = AnimID_WALK_DOWN; break;
			case EntityDirection::left:  id = AnimID_WALK_LEFT; break;
			case EntityDirection::right: id = AnimID_WALK_RIGHT; break;
			}
		}
		else if (action == EntityAction::idle) {
			switch (direction) {
			case EntityDirection::up:	 id = AnimID_IDLE_UP; break;
			case EntityDirection::down:  id = AnimID_IDLE_DOWN; break;
			case EntityDirection::left:  id = AnimID_IDLE_LEFT; break;
			case EntityDirection::right: id = AnimID_IDLE_RIGHT; break;
			}
		}
		return id;
	}
	

	SDL_FPoint calculatePlayerVelocity(const EntityState& state, const float speed) {
		SDL_FPoint vel{ 0,0 };

		if (state.getAction() == EntityAction::moving) {
			switch (state.getDirection()) {
			case EntityDirection::up:    vel.y -= speed; break;
			case EntityDirection::down:  vel.y += speed; break;
			case EntityDirection::left:  vel.x -= speed; break;
			case EntityDirection::right: vel.x += speed; break;
			default://no change but other dirs should not exist (maybe diagonals later)
				break;
			}
		}
		return vel;
	}


}
