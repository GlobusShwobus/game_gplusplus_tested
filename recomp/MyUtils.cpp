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

	SDL_FRect getNewPosition(const SDL_FRect* const position, const MovementStatus movementStatus, const int speed) {
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
	AnimationData getAnimationData(MovementStatus movementStatus, MovementStatus facingDirection) {
		//hardcoded memes, coordinates based on my textureSheet which is intended for all NPC types, fuck it just get it working
		switch (movementStatus) {
		case MS::MOVE_DOWN:  return { 0, 0,   256, 0 };
		case MS::MOVE_UP:    return { 0, 32,  256, 32 };
		case MS::MOVE_LEFT:  return { 0, 64,  256, 64 };
		case MS::MOVE_RIGHT: return { 0, 96,  256, 96 };

		case MS::MOVE_UP_LEFT:   return { 0, 96, 256, 96 };   //don't have diagonal animations
		case MS::MOVE_UP_RIGHT:  return { 0, 64, 256, 64 };   //don't have diagonal animations
		case MS::MOVE_DOWN_LEFT: return { 0, 96, 256, 96 };  //don't have diagonal animations
		case MS::MOVE_DOWN_RIGHT:return { 0, 64, 256, 64 };   //don't have diagonal animations

		case MS::MOVEMENT_STATUS_NULL:
			switch (facingDirection) {
			case MS::FACING_UP:    return { 64,128,128,128 };
			case MS::FACING_DOWN:  return { 0,128,64,128 };
			case MS::FACING_LEFT:  return { 192,128,256,128 };
			case MS::FACING_RIGHT: return { 128,128,192,128 };
			default: break;
			}
			break;
		default: break;
		}
		return { 0, 0, 0,0 };
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

	namespace Config {
		jConfig::jConfig(const char* folderPath) {

			std::ifstream in(folderPath);

			if (in.good()) {
				config = new nlohmann::json();
				in >> *config;
			}

			in.close();
		}
		bool jConfig::isValid()const {
			return config;
		}
		const nlohmann::json* const jConfig::Get()const {
			return config;
		}
		const nlohmann::json* const jConfig::Get(const char* request)const {
			if (config->contains(request)) {
				return &(*config)[request];
			}
			throw "\nwhy are you entering a wrong thingy?\n";
		}
		jConfig::~jConfig() {
			if (config) {
				delete config;
				config = nullptr;
			}
		}
	}

	namespace Rendering {
		void renderBasic(SDL_Renderer* renderer, Sprite& sprite, const Camera& cam) {
			SDL_FRect dest = cam.toCameraSpace(sprite.getDestination());
			SDL_RenderTexture(renderer, sprite.getTexture(), sprite.getSource(), &dest);
		}

	}
}