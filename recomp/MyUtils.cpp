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
	void updatePosition(Grid& grid, const SDL_FRect& updatedLocation, SDL_FRect* const previousPosition) {

		auto corners = getCorners(updatedLocation);

		for (const auto& point : corners) {
			if (!grid.isValidTile(point) || !grid.getTile(point).doesContain(TFLAG_WALKABLE)) {
				return;
			}
		}
		*previousPosition = updatedLocation;
	}

	namespace Files {
		const std::vector<std::filesystem::path> getPNGs(const char* folderPath)
		{
			std::vector<std::filesystem::path> paths;

			for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
				if (entry.path().extension() == ".png") {
					paths.push_back(entry);
				}
			}

			return paths;
		}
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
			return nullptr;
		}
		jConfig::~jConfig() {
			if (config) {
				delete config;
				config = nullptr;
			}
		}
	}

	namespace Rendering {
		void renderBasic(SDL_Renderer* renderer, const Sprite& sprite, const Camera& cam) {
			SDL_FRect dest = cam.toCameraSpace(&sprite.destination);
			SDL_RenderTexture(renderer, sprite.texture, &sprite.source, &dest);
		}

	}
}