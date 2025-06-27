#include "Entity.h"
#include "Timer.h"
#include "Camera.h"

#include "Window.h"

#include <fstream> //file reader?
#include <thread>

/*
TODO:: rework the window, want more ability to create windows and window should only care about structly window related stuff
       then we can make sprite field private (with slight consequences but that's minor)
TODO:: quadtree storage and lookup
TODO:: scene class (the big ugh)
TODO:: imgui
*/

static nlohmann::json* initJSON(const char* path) {
    //look into assurances that the path is a json so we catch exception early
    std::ifstream in(path);
    nlohmann::json* json = nullptr;
    if (in.good()) {
        json = new nlohmann::json();
        in >> *json;
    }
    in.close();

    return json;
}
class JSON_Wrapper {//temporary until file reader sometime tm
public:
    nlohmann::json* json = nullptr;

    ~JSON_Wrapper() {
        delete json;
        json = nullptr;
    }
};

int main() {
    using namespace badEngine;
    
    //configs
    JSON_Wrapper entityConfig;
    JSON_Wrapper stageConfig;
    JSON_Wrapper windowConfig;

    try {
        entityConfig.json = initJSON("entity_config.json");
        stageConfig.json  = initJSON("stage_config.json");
        windowConfig.json = initJSON("window_config.json");
    }
    catch (const std::exception& excpt) {
        printf(excpt.what());
        return -1;
    }
    //initalize SDL and RenderWIndow
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("\nCRASH:: SDL_init Failure");
        return -1;
    }
    Window window(windowConfig.json);

    //initialize EntityFactory
    EntityFactory entityFactory(entityConfig.json, window.getRenderer()) ;

    if (!entityFactory.isInitalized()) {
        entityFactory.wipeMemory();
        return -1;
    }

    //player
    //WRAP THIS SHIT UP
    Player* player = entityFactory.createPlayer(HKey::ENTITY_TYPE::PLAYER_MAIN);
    int x, y;
    SDL_GetWindowSize(window.getWindow(), &x, &y);
    Camera2D camera(x,y);
    //###################################################################

    bool gameRunning = true;
    SDL_Event event;

    while (gameRunning) {
       
        FrameTimer gameLogicTimer;
        gameLogicTimer.start();


        window.updateBegin();

        //LISTEN TO EVENTS
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                gameRunning = false;
            }
            if (event.type == SDL_EVENT_MOUSE_WHEEL) {

                if (event.wheel.y > 0) {
                    printf("zoomzoom\n");
                    camera.zoom(1.03f);
                }
                else if (event.wheel.y < 0) {
                    camera.zoom(0.97f);
                }
            }

        }
        //###############################################################################

        //BULLSHIT

        player->WASD_PlayerVelocity(5.0f);
        player->state.isMoving = isMoving(player->hitbox.velocity);
        SMS::Facing safetyCheck = facingDirection(player->hitbox.velocity);
        
        if (safetyCheck != SMS::Facing::UNKNOWN) {
            player->state.facing = safetyCheck;
        }

        TSA::AnimationID animationID = animationIDTable(player->state);
        
        if (animationID != TSA::AnimationID::UNKNOWN && player->sprite.getCurrentAnimationID() != animationID) {
            if (!player->sprite.setNewAnimation(animationID)) {
                printf("\nerror: attempt to set invalid animation id");
            }
        }
        player->sprite.play();
        setCoordinates(player->hitbox);
        //###############################################################################
        

        //COLLISION
        //...        
        //#################################################################################
        
        //CAMERA
        camera.centerOn(player->hitbox.rectangle.x, player->hitbox.rectangle.y);
        //window.updateCamera(player->hitbox.rectangle, world.rectangle);//wrong clamp size btw
        //#################################################################################

        //MAIN LOGIC ENDING
        TSA::setTTransferField_coordinates(player->sprite.getAnimatedTextureSource(), player->sprite.source);
        TSA::setTTransferField_coordinates(player->hitbox.rectangle, player->sprite.dest);

        SDL_FRect adjustedDest = player->sprite.dest;
        camera.worldToScreen(player->sprite.dest.x, player->sprite.dest.y, adjustedDest.x, adjustedDest.y);
        adjustedDest.w *= camera.getScaleX();
        adjustedDest.h *= camera.getScaleY();
        SDL_RenderTexture(window.getRenderer(), player->sprite.texture.get(), &player->sprite.source, &adjustedDest);
        player->hitbox.velocity = { 0.f, 0.f };//temporary bullshit

        window.updateEnd();
        gameLogicTimer.end();
        //#################################################################################
        
        //HANDLE TASKS BETWEEN FRAMES
        if (gameLogicTimer.isSpareTime()) {

            const auto spareTime = gameLogicTimer.getMaxFrameDuration_milliseconds() - gameLogicTimer.getDuration_millisecond();

            Timer supplementaryLogicTimer;
            supplementaryLogicTimer.start();

            //DO SHIT HERE.. ALSO IF LOOPITY ACTION THEN THE TIMER SHOULD PROBABLY BE PART OF THE LOOP INSTEAD 

            //#################################################################################

            supplementaryLogicTimer.end();

            const auto remainingTime = spareTime - supplementaryLogicTimer.getDuration_millisecond();
            
            if (remainingTime.count() > 0) {
                std::this_thread::sleep_for(remainingTime);
            }
        }
        //#################################################################################
        
    }
    SDL_Quit();
    delete player;

    return 0;
}


//std::vector<std::pair<int, float>> collisions;
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


/*
RECTANGLE (or any entity) PROPER COLLISION TEMPLATE, 2 STAGED

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
*/