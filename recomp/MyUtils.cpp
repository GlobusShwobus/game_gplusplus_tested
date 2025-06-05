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
	bool WASD_PlayerVelocity(SDL_FPoint& velocity, const float moveSpeed) {
		const auto* keystate = SDL_GetKeyboardState(nullptr);

		bool w = keystate[SDL_SCANCODE_W];
		bool a = keystate[SDL_SCANCODE_A];
		bool s = keystate[SDL_SCANCODE_S];
		bool d = keystate[SDL_SCANCODE_D];

		bool outcome = false;

		if      (w) { velocity.y -= moveSpeed; outcome = true; }
		else if (a) { velocity.x -= moveSpeed; outcome = true; }
		else if (s) { velocity.y += moveSpeed; outcome = true; }
		else if (d) { velocity.x += moveSpeed; outcome = true; }

		return outcome;
	}
	Direction directionOfVelocity(const SDL_FPoint& velocity) {

		static constexpr float PI = 3.1415927410125732421875;
		float angle = std::atan2(velocity.y, velocity.x) * 180.f / PI;
		if (angle < 0)angle += 360.f;

		int index = (int)std::round(angle / 90.f) % 4;

		return (Direction)index;
	}

	AnimID movableObjectSheetIDTable(const Direction movementDir, Uint64& flags) {
		AnimID id = 0;

		bool isMoving = EntityEvent::containsEvent(flags, MyPersonalEvents::MPE_movingObject);
		//other flags here

		if (isMoving) {
			switch (movementDir) {
			case Direction::up:	   id = AnimID_WALK_UP;    break;
			case Direction::down:  id = AnimID_WALK_DOWN;  break;
			case Direction::left:  id = AnimID_WALK_LEFT;  break;
			case Direction::right: id = AnimID_WALK_RIGHT; break;
			}
		}
		else {
			switch (movementDir) {
			case Direction::up:	   id = AnimID_IDLE_UP;    break;
			case Direction::down:  id = AnimID_IDLE_DOWN;  break;
			case Direction::left:  id = AnimID_IDLE_LEFT;  break;
			case Direction::right: id = AnimID_IDLE_RIGHT; break;
			}
		}
		return id;
	}
}
