#include "EntityFactory.h"
#include "Window.h"
#include "Grid.h"


#include "MyUtils.h"

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

/*

*MyUtils::MoveScript is dogshit. Velocity has no place in tranform class and should be held in physics class instead.
*MyUtils::WASD_state is unique to player because only players moves on key press, implement it into the player class

    reflect velocity
    then test bouncing off off each other
    then test if type A and type B instead of bounce type A stops and B bounces back
    then test different coefficient additions with speed and mass determining how much to bounce

    lastly store important data what i wished to save into transform

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

    //TESTCODE
    SDL_FRect worldBB{ 0,0,2560,1440 };
    RandomNumberGenerator rnggen;
    std::vector<SDL_FRect> rects;
    for (int i = 0; i < 1000; i++) {
        SDL_FRect rect;
        rect.x = rnggen.getRand(0, 2560);
        rect.y = rnggen.getRand(0, 1440);
        rect.w = rnggen.getRand(5, 16);
        rect.h = rnggen.getRand(5, 16);

        rects.push_back(rect);
    }
    //###################################################################
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

        //VELOCITY LOGIC
        SDL_FPoint newPlayerVel = MyUtils::calculatePlayerVelocity(player->state, 5);
        player->transform.setVelocity(newPlayerVel);
        //#################################################################################

        //CAMERA
        //(camera must be applied AFTER the entity moves, otherwise the camera falls behind by a frame)
        window.updateCamera(player->transform.getRect(), worldBB);
        //#################################################################################

        //ANIMATION
        if (player->state.containsEvent(directionChange)) {
            player->animControlls.setNewReel(MyUtils::getReelOnState(player->state.getAction(), player->state.getDirection()));
        }
        player->animControlls.moveFrame();
        //#################################################################################

        //COLLISION
        CollisionSweptResult eCol;
        bool foundCollision = false;
        float earliestTime = 1.0f;

        for (auto& eachRect : rects) {
            CollisionSweptResult pCol;
            if (player->transform.sweptAABB_adjusted(eachRect, pCol)) {
                if (pCol.tHitNear < earliestTime) {
                    earliestTime = pCol.tHitNear;
                    eCol = pCol;
                    foundCollision = true;
                }
            }
        }
        if (foundCollision) {
            player->transform.clampOnCollision(eCol);
        }
        else {
            player->transform.updatePosUnrestricted();
        }
        //###############################################################################

        player->applySourceBoxToRenderBox();
        player->applyCollisionBoxToRenderBox();

        //TESSTCODE
        SDL_SetRenderDrawColor(window.getRenderer(), 255, 0, 0, 255);
        for (auto& eachrect : rects) {
            window.drawBasicRect(&eachrect);
        }
        SDL_SetRenderDrawColor(window.getRenderer(), 0, 0, 0, 255);
        //#######################

        window.drawTexture(player->texture, &player->textureSrc, &player->textureDest);

        //MAIN LOGIC ENDING
        player->state.flushEvents();
        window.updateEnd();
        ////#################################################################################

   
        //HANDLE TASKS BETWEEN FRAMES
        if (window.shouldDelay()) {

            Uint64 delayTime = window.getDelayDuration();
            Uint64 startDelay = SDL_GetTicks();

            //DO SHIT HERE

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
