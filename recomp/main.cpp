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
    SDL_FRect worldBBr{ 0,0,500,500 };
    Transform worldBB;
    worldBB.rect = worldBBr;
    RandomNumberGenerator rnggen;
    std::vector<Transform> rects;

    for (int i = 0; i < 25; i++) {
        SDL_FRect frect;
        SDL_FPoint fvel;

        frect.x = rnggen.getRand(0, 400);
        frect.y = rnggen.getRand(0, 400);
        frect.w = rnggen.getRand(32, 32);
        frect.h = rnggen.getRand(32, 32);

        fvel.x = rnggen.getRand(-2, 2);
        fvel.y = rnggen.getRand(-2, 2);

        Transform afafa;
        afafa.rect = frect;
        afafa.velocity = fvel;

        rects.push_back(afafa);
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
        //VELOCITY AND FRAME
        if (MyUtils::WASD_PlayerVelocity(player->transform.velocity, 5)) {
            player->events.setEvent(MPE_movingObject | MPE_checkDirection);
        }
        player->events.setEvent(MPE_checkAnimation);//otherwise if idle it doesn't change it

        if (player->events.containsEvent(MPE_checkDirection)) {
            player->direction = MyUtils::directionOfVelocity(player->transform.velocity);
        }

        if (player->events.containsEvent(MPE_checkAnimation)) {
            AnimID id = MyUtils::movableObjectSheetIDTable(player->direction, player->events.events);
            player->animControlls.setIfNew(id);
        }

        player->animControlls.moveFrame();
        //###############################################################################
        

        //COLLISION
        std::vector<std::pair<int, float>> collisions;

        for (int i = 0; i < rects.size();i++) {
            SDL_FPoint contactP{ 0,0 }, contactN{ 0,0 };
            float hitTime = 0;
            if (player->transform.projectionHitBoxAdjusted(rects[i].rect, contactP, contactN, hitTime)) {
                collisions.push_back({ i, hitTime });
            }
        }
        std::sort(collisions.begin(), collisions.end(), [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
            return a.second < b.second;
            });
        for (auto& j : collisions) {
            SDL_FPoint contactP{ 0,0 }, contactN{ 0,0 };
            float hitTime = 0;
            if (player->transform.projectionHitBoxAdjusted(rects[j.first].rect, contactP, contactN, hitTime)) {
                player->transform.clampNextTo(rects[j.first].rect, contactN);
                player->transform.clearVelocity();
            }
        }
        player->transform.updatePos();
        //###############################################################################
        
        //COLLISION OTHER MOVING OBJECTS
        for (int i = 0; i < rects.size(); i++) {
            for (int j = i + 1; j < rects.size(); j++) {
                SDL_FPoint contactP{ 0,0 }, contactN{ 0,0 };
                float hitTime = 0;

                if (rects[i].projectionHitBoxAdjusted(rects[j].rect, contactP, contactN, hitTime)) {
                    rects[i].clampNextTo(rects[j].rect, contactN);
                    rects[i].reflectVelocity(contactN);
                    rects[i].flipNormalized(contactN);
                    rects[j].reflectVelocity(contactN);
                }
                if (!worldBB.containsRect(rects[i].rect)) {
                    rects[i].velocity.x *= -1;
                    rects[i].velocity.y *= -1;
                }
            }
        }
        for (auto& each : rects) {
            each.updatePos();
        }
        ///////////////////////////////////

        //CAMERA
        window.updateCamera(player->transform.rect, worldBBr);//wrong clamp size btw
        //#################################################################################

        //TESSTCODE
        SDL_SetRenderDrawColor(window.getRenderer(), 255, 0, 0, 255);
        for (auto& eachrect : rects) {
            window.drawBasicRect(&eachrect.rect);
        }
        SDL_SetRenderDrawColor(window.getRenderer(), 0, 0, 0, 255);
        //#######################


        //MAIN LOGIC ENDING
        player->applySourceBoxToRenderBox();
        player->applyCollisionBoxToRenderBox();
        window.drawTexture(player->texture, &player->textureSrc, &player->textureDest);
        player->events.flushEvents();
        player->transform.velocity = { 0.0f,0.0f };//temporary
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
