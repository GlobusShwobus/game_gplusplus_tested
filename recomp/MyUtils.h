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

	//eventually integrate fully into transform OR make more options with transform funcs that a script handles
	void moveScriptBasic(Transform& transform, NPCState& state, const float moveSpeed);


	AnimID getReelOnState(const NPCState& state);
}