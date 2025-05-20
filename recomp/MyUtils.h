#pragma once


#include <array>
#include <fstream>

#include "json.hpp"
#include "SDL3/SDL.h"
#include "Grid.h"
#include "BasicComponents.h"

namespace MyUtils {

	std::array<SDL_FPoint, 4> getCorners(const SDL_FRect& position);
    std::pair<NPCState::State, NPCState::Facing> getWASDState();
    nlohmann::json* initJSON(const char* path);

    SDL_FRect getNewPosition(const SDL_FRect* const position, const MovementStatus movementStatus, const float speed);
	void updatePosition(Grid& grid, const SDL_FRect& updatedLocation, SDL_FRect* const previousPosition);
    ClipID getClipBasedOnMovement(const Movement movementData);
}