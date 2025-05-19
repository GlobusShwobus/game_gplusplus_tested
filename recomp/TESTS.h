#pragma once

#include "Window.h"
#include "Grid.h"

namespace TESTS {


    void SetNoWalkingZone(Grid& grid);
    void TestNoWalkingZone(SDL_Renderer* renderer);
}