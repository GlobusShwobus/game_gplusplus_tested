#include "EntityFactory.h"
#include "Window.h"
#include "Grid.h"


#include "MyUtils.h"
#include "TESTS.h"

/*

TODO:: TRANSFORM COMPONENT:
       currently sprite destination is used for pos checking, instead there should be dedicated transform
       similarly to how sprites source information is derived from animation controller class, sprite destination will be derived from transform, meaning it should be applied at the very end before drawing

TODO:: COLLISIONS:
       horizontal overlap == if top of rect A is higher than bottom of rect B, and top of rect B is higher than bottom of rect A;  [y1<y2+h2  &&  y2<y1+h1] ==> less than sign because we start top left
       vertical overlap == if left of rect A is to the left of the right side of rect B, and if the left side of rect B is to the left of the right side of rect A; [x1<x2+w2  &&  x2<x1+w1]
       if overlap both horizontally AND vertically == collision
       store data for how much was collided (x, y), probably need -+ to know from which direction

TODO:: add physics struct that holds speed, mass, maybe other and use it instead of basic float speed

TODO:: TEST: set up some random npc spawning and make them move randomly and basically just test the collision
       give player some weight, or enemies some differing weights, calcualte the differential in overlap, speed and weight
       values to determine how much and were to push back, 
       DESIRED RESULTS:: 
       for the transform to be able to handle collision and outcomes, that's it, the test code will be junk
TODO:: CLEAN UP!!! 
       variable names
       behavior encapsulation
       at least try to be const correct
       if possible some optimization
       of note:: entities are in kind of a free for all with all vars being public and names are all over the place

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
    EntityFactory entityFactory;
    if (!entityFactory.initFactory(entityConfig, window.getRenderer())) {
        return -1;
    }

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
    //WRAP THIS SHIT UP
    PlayerID id = HASH("player_version1");
    Player* player = entityFactory.createPlayer(id);
    if (!player) {
        printf("player nullptr GG WP\n");
        return -1;
    }

    //TEST CODE
    TESTS::RandomNumberGenerator rng;
    TESTS::ENTITY_MANAGER_TEST poop;
    //###################################################

    
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
        MyUtils::moveScriptBasic(player->transform, player->state, player->movementSpeed);
        player->transform.clampPosition(0,0,2560,1440);//currently map does not exist
        //#################################################################################

        //CAMERA
        //(camera must be applied AFTER the entity moves, otherwise the camera falls behind by a frame)
        window.updateCamera(player->transform.getPosition(), player->transform.getSize(), { 0,0,2560,1440 });
        //#################################################################################

        //ANIMATION
        if (player->state.didChangeOccur()) {
            player->animControlls.setNewReel(MyUtils::getReelOnState(player->state));
            player->state.handeledChange();
        }

        player->animControlls.moveFrame();
        //#################################################################################

        player->animControlls.applySourceFromFrame(player->textureSrc);//MAKE THIS FUNCTION PART OF PLAYER NOT ANIMCONTROLLS OR UTILITY
        player->transform.applyDestinationTexture(player->textureDest);//MAKE THIS FUNCTION PART OF PLAYER NOT TRANSFORM OR UTILITY


        window.drawTexture(player->texture, &player->textureSrc, &player->textureDest);

        //pooptest
        for (auto& each : poop.getEnemies()) {
            each->transform.applyDestinationTexture(each->textureDest);
            window.drawTexture(each->texture, &each->textureSrc, &each->textureDest);
        }
        //#####

        window.updateEnd();

        //TESTCODE FOR CREATING ENTITIES
        const auto* meme = SDL_GetKeyboardState(nullptr);
        bool k = meme[SDL_SCANCODE_K];
        bool l = meme[SDL_SCANCODE_L];
        bool m = meme[SDL_SCANCODE_M];
        if (k) {
            TESTS::ASK_10_ENEMIES(poop);
        }
        if (l) {
            TESTS::GIVE_RANDOM_POSITIONS_TO_ENEMIES(poop, rng);
        }
        if (m) {
            TESTS::CLEAR_ALL_ENTITIES(poop);
        }
        //#########################
   
        //DO SHIT HERE LIKE CALLING "new" and "delete"
        if (window.shouldDelay()) {

            Uint64 delayTime = window.getDelayDuration();
            Uint64 startDelay = SDL_GetTicks();

            //DO SHIT HERE
            poop.update(entityFactory, 1);
            poop.removeEnemy();
            //#################################################################################

            Uint32 remaining = delayTime - (SDL_GetTicks() - startDelay);

            if (remaining > 0) {
                SDL_Delay(remaining);
            }
        }
        //#################################################################################
    }


    SDL_Quit();

    //OR GET RID OF THEM AFTER USING THEM, FOOD FOR THOUGHT LATER
    delete entityConfig;
    delete stageConfig;
    delete windowConfig;

    delete player;

    return 0;
}
