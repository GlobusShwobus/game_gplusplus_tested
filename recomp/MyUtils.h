#pragma once


#include <array>
#include <fstream>

#include "json.hpp"
#include "SDL3/SDL.h"
#include "Grid.h"
#include "Player.h"

namespace MyUtils {

	std::array<SDL_FPoint, 4> getCorners(const SDL_FRect& position);
    SDL_FRect getNewPosition(const SDL_FRect* const position, const MovementStatus movementStatus, const float speed);
	void updatePosition(Grid& grid, const SDL_FRect& updatedLocation, SDL_FRect* const previousPosition);
    nlohmann::json* initJSON(const char* path);

    ClipID getClipBasedOnMovement(const Movement movementData);

    namespace Rendering {
        //later sepparate logic for player rendering and world objects as that will enable ignoring rendering outside of camera area
        //for now, don't have NPCs for that to set up
        void renderBasic(SDL_Renderer* renderer, Sprite& sprite, const Camera& cam);

    }
}