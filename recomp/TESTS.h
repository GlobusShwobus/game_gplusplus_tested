#pragma once

#include "RenderWindow.h"
#include "Grid.h"

namespace TESTS {


    void SetNoWalkingZone(Grid& grid);
    void TestNoWalkingZone(SDL_Renderer* renderer);
}