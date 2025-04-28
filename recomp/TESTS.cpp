#include "TESTS.h"

void TESTS::SetNoWalkingZone(Grid& grid) {
    const float startX = 32 * 5;
    const float startY = 32 * 5;

    const float width = 32;
    const float height = 32;

    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            auto& tile = grid.getTile({ startX + (32 * x), startY + (32 * y) });
            tile.removeData(TFLAG_WALKABLE);
        }
    }
}

void TESTS::TestNoWalkingZone(SDL_Renderer* renderer) {

    const int startX = 32 * 5;
    const int startY = 32 * 5;
    const int width = 32 * 5;
    const int height = 32 * 5;

    SDL_FRect rect = { startX, startY, width, height };

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}