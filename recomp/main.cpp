#include "EntityFactory.h"
#include "Window.h"
#include "Grid.h"


#include "MyUtils.h"


int main() {

    
    //configs
    nlohmann::json* entityConfig    = nullptr;
    nlohmann::json* stageConfig     = nullptr;
    nlohmann::json* windowConfig    = nullptr;

    try {
        entityConfig = MyUtils::initJSON("entity_config.json");
        stageConfig = MyUtils::initJSON("stage_config.json");
        windowConfig = MyUtils::initJSON("window_config.json");
    }
    catch (const std::exception& excpt) {
        printf(excpt.what());
        delete entityConfig;
        delete stageConfig;
        delete windowConfig;
        return -1;
    }
    //initalize SDL and RenderWIndow
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("\nCRASH:: SDL_init Failure");
        return -1;
    }
    Window window(windowConfig);

    //initialize EntityFactory
    EntityFactory entityFactory(entityConfig, window.getRenderer());

    //###################################################
    // 
    // Grid to become scene, which holds the map and any entities associated with it.
    // 
    // 
    // Sprite worldMap = textureManager.createSprite(SpriteID::world_map);
     Grid grid = Grid(2560, 1440);
    // 
    // 
    //#####################################################################


    //player
    Player* player = entityFactory.createPlayer("player_version1");


    bool gameRunning = true;
    SDL_Event event;

    while (gameRunning) {

        window.updateBegin();


        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                gameRunning = false;
            }
        }
        MyUtils::WASD_state(player->state);

        //MOVEMENT
        
        if (player->state.didStateChange()) {
            put move logic here, dingus
        }

        SDL_FRect* pos = player->getPosition();
        SDL_FRect newPos = MyUtils::getNewPosition(*pos, player->state, player->movementSpeed);
        MyUtils::updatePosition(grid, newPos, pos);//UNITE ALL FUNCTIONALITY INTO 1, GET POS, NEW POS, UPDATE POS, CAN MAKE IT LEANER
        //#################################################################################

        //CAMERA
        window.getCamera()->setFocusPoint(player->getPosition());
        SDL_FRect poopa{ 0,0,2560,1440 };//GET RID OF THIS LATER WITH SCENE SET UP
        window.getCamera()->clampTo(&poopa);
        //#################################################################################

        //ANIMATION
        const ClipID clip = MyUtils::getClipBasedOnMovement(player.movement);
        player.sprite.play(clip);
        //#################################################################################


        window.drawSprite(&player->sprite);


        window.updateEnd();
    }


    window.~Window();
    SDL_Quit();


    //OR GET RID OF THEM AFTER USING THEM, FOOD FOR THOUGHT LATER
    delete entityConfig;
    delete stageConfig;
    delete windowConfig;

    delete player;

    return 0;
}
