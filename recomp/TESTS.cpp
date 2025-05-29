#include "TESTS.h"

namespace TESTS {
    void ASK_10_ENEMIES(ENTITY_MANAGER_TEST& entman) {
        for (int i = 0; i < 10; i++) {
            entman.addEnemy("enemy_spear1");
        }
    }

    void GIVE_RANDOM_POSITIONS_TO_ENEMIES(ENTITY_MANAGER_TEST& entman, RandomNumberGenerator& lolw) {
        const auto& enemies = entman.getEnemies();

        for (auto& each : enemies) {
            int x = lolw.getRand(0, 500);
            int y = lolw.getRand(0, 500);

            each->transform.setPosStaight(x, y);
        }

    }
    void CLEAR_ALL_ENTITIES(ENTITY_MANAGER_TEST& entman) {
        for (auto& lol : entman.getEnemies()) {
            lol->isActivated = false;
        }
    }
    void SetNoWalkingZone(Grid& grid) {
        const float startX = 32 * 5;
        const float startY = 32 * 5;

        const float width = 32;
        const float height = 32;

        for (int x = 0; x < 5; x++) {
            for (int y = 0; y < 5; y++) {
                const SDL_FPoint point{ startX + (32 * x), startY + (32 * y) };
                auto& tile = grid.getTile(point);
                tile.removeData(TFLAG_WALKABLE);
            }
        }
    }

    void TestNoWalkingZone(SDL_Renderer* renderer) {

        const int startX = 32 * 5;
        const int startY = 32 * 5;
        const int width = 32 * 5;
        const int height = 32 * 5;

        SDL_FRect rect = { startX, startY, width, height };

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}
