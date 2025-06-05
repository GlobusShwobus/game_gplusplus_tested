#pragma once

#include "Grid.h"//junk atm

#include <fstream>
#include "json.hpp"
#include "SDL3/SDL.h"
#include "BasicComponents.h"
#include <math.h>

namespace MyUtils {

	std::array<SDL_FPoint, 4> getCorners(const SDL_FRect& position);
    nlohmann::json* initJSON(const char* path);
	bool WASD_PlayerVelocity(SDL_FPoint& velocity, const float moveSpeed);
	Direction directionOfVelocity(const SDL_FPoint& velocity);
	AnimID movableObjectSheetIDTable(const Direction movementDir, Uint64& flags);
}