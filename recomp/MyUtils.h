#pragma once


#include <array>
#include <fstream>
#include <filesystem>

#include "json.hpp"
#include "SDL3/SDL.h"
#include "Grid.h"

namespace MyUtils {

	std::array<SDL_FPoint, 4> getCorners(const SDL_FRect& position);
	void updatePosition(Grid& grid, const SDL_FRect& updatedLocation, SDL_FRect* const previousPosition);

	namespace Files {
		const std::vector<std::filesystem::path> getPNGs(const char* folderPath);
	}

	namespace Config {
        
        class jConfig {
            nlohmann::json* config = nullptr;

        public:
            jConfig() = delete;
            jConfig(const char* folderPath);
            bool isValid()const;
            const nlohmann::json* const Get()const;
            const nlohmann::json* const Get(const char* request)const;

            ~jConfig();
            private:

                jConfig(const jConfig& copy) = delete;
                jConfig(jConfig&& move)noexcept = delete;
                jConfig& operator=(const jConfig& assign) = delete;
                jConfig& operator=(jConfig&& assign)noexcept = delete;
        };
	}
}