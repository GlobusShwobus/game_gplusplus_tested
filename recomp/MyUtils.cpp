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

	SDL_FRect getNewPosition(const SDL_FRect* const position, const int movementStatus, const int speed) {
		SDL_FRect newPos = *position;

		if (movementStatus & MovementStatusFlags::MFLAG_KEY_W) {
			newPos.y -= speed;
		}
		if (movementStatus & MovementStatusFlags::MFLAG_KEY_S) {
			newPos.y += speed;
		}
		if (movementStatus & MovementStatusFlags::MFLAG_KEY_A) {
			newPos.x -= speed;
		}
		if (movementStatus & MovementStatusFlags::MFLAG_KEY_D) {
			newPos.x += speed;
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