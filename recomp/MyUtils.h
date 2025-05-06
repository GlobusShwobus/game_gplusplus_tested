#pragma once


#include <array>
#include <fstream>
#include <filesystem>

#include "json.hpp"
#include "SDL3/SDL.h"
#include "Grid.h"
#include "Player.h"

namespace MyUtils {

	std::array<SDL_FPoint, 4> getCorners(const SDL_FRect& position);
    SDL_FRect getNewPosition(const SDL_FRect* const position, const int movementStatus, const int speed);
	void updatePosition(Grid& grid, const SDL_FRect& updatedLocation, SDL_FRect* const previousPosition);

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
    namespace Rendering {
        //later sepparate logic for player rendering and world objects as that will enable ignoring rendering outside of camera area
        //for now, don't have NPCs for that to set up
        void renderBasic(SDL_Renderer* renderer, Sprite& sprite, const Camera& cam);

    }
}