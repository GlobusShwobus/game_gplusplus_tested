#include "EntityFactory.h"
#include "Window.h"
#include "Grid.h"


#include "MyUtils.h"

/*

basic ray cast collision detect -> get a bool and a distance out of it
pepega AABB collision detection -> get normal and overlap rect
done

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
#include "vec2.h"

int main() {
    using namespace badEngine;
    
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
    Transform worldBB;
    worldBB.rect = { 0,0,500,500 };
    RandomNumberGenerator rng;

    std::vector<Transform> formers;
    for (int i = 0; i < 50; i++) {
        SDL_FRect rect;
        rect.x = rng.getRand(0, 500);
        rect.y = rng.getRand(0, 500);
        rect.w = rng.getRand(5, 32);
        rect.h = rng.getRand(5, 32);

        SDL_FPoint vel;
        vel.x = rng.getRand(1, 5);
        vel.y = rng.getRand(1, 5);

        Transform transform(rect, vel);

        formers.push_back(transform);
    }
    CCP::HitBox hb1({ 10,0,50,50 }, { 0,5 });
    CCP::HitBox hb2({0,500,50,50},{0,-5});
    bool stop = false;

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

        //for (int i = 0; i < rects.size();i++) {
        //    SDL_FPoint contactP{ 0,0 }, contactN{ 0,0 };
        //    float hitTime = 0;
        //    if (player->transform.projectionHitBoxAdjusted(rects[i].rect, contactP, contactN, hitTime)) {
        //        collisions.push_back({ i, hitTime });
        //    }
        //}
        //std::sort(collisions.begin(), collisions.end(), [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
        //    return a.second < b.second;
        //    });
        //for (auto& j : collisions) {
        //    SDL_FPoint contactP{ 0,0 }, contactN{ 0,0 };
        //    float hitTime = 0;
        //    if (player->transform.projectionHitBoxAdjusted(rects[j.first].rect, contactP, contactN, hitTime)) {
        //        player->transform.clampNextTo(rects[j.first].rect, contactN);
        //        player->transform.clearVelocity();
        //    }
        //}
        //player->transform.updatePos();
        
        //###############################################################################
        
        //COLLISION TEMPLATE (SAVE IT LATER SOMEWHERE)
        SDL_FPoint cp1{ 0,0 };
        SDL_FPoint cn1{ 0,0 };
        float hitTime1 = 0;

        struct Collider {
            Transform* rect1 = nullptr;
            Transform* rect2 = nullptr;
            SDL_FPoint contactNormal{ 0,0 };
            //float hitTime = 0;
        };
        std::vector<Collider> colliders;

        for (int i = 0; i < formers.size(); i++) {
            for (int j = i + 1; j < formers.size(); j++) {
                SDL_FPoint cp1{ 0,0 };
                SDL_FPoint cn1{ 0,0 };
                float hitTime1 = 0;

                if (formers[i].dynamicSweptAABB(formers[j], cp1, cn1, hitTime1)) {
                    colliders.push_back({&formers[i], &formers[j], cn1});
                }
            }
        }
        for (auto& move : formers) {
            move.noResolutionMove();
        }

        for (auto& col : colliders) {
            if (col.rect1->staticAABBOverlap(*col.rect2)) {
                SDL_FPoint normal = col.contactNormal;
                col.rect1->reflectVelocity(normal);
                Transform::flipNormalized(normal);
                col.rect2->reflectVelocity(normal);

            }
        }

        for (auto& each : formers) {
            if (!worldBB.containsRect(each.rect)) {
                Transform::clampInOf(worldBB.rect, each.rect);
                each.velocity.x *= -1;
                each.velocity.y *= -1;
            }
        }

        SDL_Rect output{ 0,0,0,0 };
        if (!stop) {
            if (CCP::collisionEnhancedAABB(hb1, hb2, &output)) {
                CCP::resolveAABB_overlapSecond(hb1.rectangle, output);
                stop = true;
            }
            else {
                hb1.rectangle = CCP::addXY(hb1.rectangle, hb1.velocity);
                hb2.rectangle = CCP::addXY(hb2.rectangle, hb2.velocity);
            }
        }
        ///////////////////////////////////

        //CAMERA
        window.updateCamera(player->transform.rect, worldBB.rect);//wrong clamp size btw
        //#################################################################################

        //TESSTCODE
       // SDL_SetRenderDrawColor(window.getRenderer(), 0, 0, 255, 255);
       // for (auto& each : formers) {
       //     window.drawBasicRect(&each.rect);
       // }
       // SDL_SetRenderDrawColor(window.getRenderer(), 0, 0, 0, 255);
        SDL_SetRenderDrawColor(window.getRenderer(), 0, 0, 255, 255);
        window.drawBasicRect(&hb1.rectangle);
        SDL_SetRenderDrawColor(window.getRenderer(), 255, 0, 0, 255);
        window.drawBasicRect(&hb2.rectangle);
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
