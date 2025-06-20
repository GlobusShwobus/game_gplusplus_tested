#pragma once


#include <array>
#include <fstream>

#include "json.hpp"
#include "SDL3/SDL.h"
#include "Grid.h"
#include "BasicComponents.h"

namespace MyUtils {

	std::array<SDL_FPoint, 4> getCorners(const SDL_FRect& position);
    void WASD_state(EntityState& state);
    nlohmann::json* initJSON(const char* path);

	//eventually integrate fully into transform OR make more options with transform funcs that a script handles
	void moveScriptBasic(Transform& transform, const EntityState& state, const float moveSpeed);


	AnimID getReelOnState(EntityAction action, EntityDirection direction);
}