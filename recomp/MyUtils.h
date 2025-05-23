#pragma once


#include <array>
#include <fstream>

#include "json.hpp"
#include "SDL3/SDL.h"
#include "Grid.h"
#include "BasicComponents.h"

namespace MyUtils {

	std::array<SDL_FPoint, 4> getCorners(const SDL_FRect& position);
    void WASD_state(NPCState& state);
    nlohmann::json* initJSON(const char* path);

	SDL_FRect getNewPosition(const SDL_FRect* const current, const NPCState::Facing facing, const float speed);
	void doMovement(Grid& grid, SDL_FRect* const currentPosition, const NPCState& state, const float speed);


	AnimID getReelOnState(const NPCState& state);
}