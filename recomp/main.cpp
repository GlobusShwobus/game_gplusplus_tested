#include "EntityFactory.h"
#include "Window.h"
#include "Grid.h"


#include "MyUtils.h"

/*

TODO:: TRANSFORM COMPONENT:
       currently sprite destination is used for pos checking, instead there should be dedicated transform
       similarly to how sprites source information is derived from animation controller class, sprite destination will be derived from transform, meaning it should be applied at the very end before drawing
       no need to store transform in the config- it can be desrived from sprite dest (so the opposite), but it must be constructed correctly in the constructor

TODO:: COLLISIONS:
       horizontal overlap == if top of rect A is higher than bottom of rect B, and top of rect B is higher than bottom of rect A;  [y1<y2+h2  &&  y2<y1+h1] ==> less than sign because we start top left
       vertical overlap == if left of rect A is to the left of the right side of rect B, and if the left side of rect B is to the left of the right side of rect A; [x1<x2+w2  &&  x2<x1+w1]
       if overlap both horizontally AND vertically == collision
       store data for how much was collided (x, y), probably need -+ to know from which direction

TODO:: Grid to scene
       retains grid functionality
       stores the map and data describing what areas are walkable what not
       stores entities
       handles batch creation
       handles batch clean up
       (massive bonus points for batch behavior like movement/collision etc)
       saves data in a config where what is
       may need to store additional event type information for scrips but not necessarily scripts themselves

TODO:: window frame counter and camera require a better constructor not init function, looks fugly
TODO:: warp json objects so they get a more secure/automatic clean up when they're not desired any longer
TODO:: decide wtf to do with the player, should it or should it not be at least in some shortcut way be tied to scenes?

*/

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
        MyUtils::doMovement(grid, player->sprite.getDestination(), player->state, player->movementSpeed);
        //#################################################################################

        //CAMERA
        window.getCamera()->setFocusPoint(player->transform.getCurrentPos(), player->transform.getSize());
        SDL_FRect poopa{ 0,0,2560,1440 };//GET RID OF THIS LATER WITH SCENE SET UP
        window.getCamera()->clampTo(&poopa);
        //#################################################################################

        //ANIMATION
        if (player->state.didChangeOccur()) {
            player->animControlls.setNewReel(MyUtils::getReelOnState(player->state));
            player->state.handeledChange();
        }

        player->animControlls.moveFrame();
        player->animControlls.applySourceFromFrame(&player->texture.source);
        //#################################################################################


        window.drawTexture(&player->texture);

        window.updateEnd();
    }


    SDL_Quit();

    //OR GET RID OF THEM AFTER USING THEM, FOOD FOR THOUGHT LATER
    delete entityConfig;
    delete stageConfig;
    delete windowConfig;

    delete player;

    return 0;
}
