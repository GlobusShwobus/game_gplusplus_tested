#include "EntityFactory.h"
#include "Window.h"
#include "Grid.h"

#include <fstream> //file reader?

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
    EFM::EntityFactory entityFactory(entityConfig.json, window.getRenderer()) ;

    if (!entityFactory.isInitalized()) {
        entityFactory.wipeMemory();
        return -1;
    }

    //player
    //WRAP THIS SHIT UP
    EDM::Player* player = entityFactory.createPlayer(HKey::ENTITY_TYPE::PLAYER_MAIN);
    player->sprite.debugPRINT();
    //TESTCODE
    CCP::HitBox world(0, 0, 2560, 1440);
    //###################################################################

    bool gameRunning = true;
    SDL_Event event;

    while (gameRunning) {

        window.updateBegin();

        //LISTEN TO EVENTS
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                gameRunning = false;
            }
        }
        //###############################################################################

        //BULLSHIT
        player->WASD_PlayerVelocity(5.0f);
        player->state.isMoving = EDM::isMoving(player->hitbox.velocity);
        player->state.facing = EDM::facingDirection(player->hitbox.velocity);

        TSA::AnimationID animationID = TSA::animationIDTable(player->state);
        
        if (animationID != TSA::AnimationID::UNKNOWN && player->sprite.getCurrentAnimationID() != animationID) {
            if (!player->sprite.setNewAnimation(animationID)) {
                printf("\nerror: attempt to set invalid animation id");
            }
        }
        player->sprite.play();
        EDM::setCoordinates(player->hitbox);
        //###############################################################################
        

        //COLLISION
        //...        
        //#################################################################################
        
        //CAMERA
        window.updateCamera(player->hitbox.rectangle, world.rectangle);//wrong clamp size btw
        //#################################################################################

        //MAIN LOGIC ENDING
        TSA::setTTransferField_coordinates(player->sprite.getAnimatedTextureSource(), player->sprite.source);
        TSA::setTTransferField_coordinates(player->hitbox.rectangle, player->sprite.dest);

        window.drawTexture(player->sprite.texture, &player->sprite.source, &player->sprite.dest);
        player->hitbox.velocity = { 0.0f,0.0f };//temporary bullshit
        window.updateEnd();
        //#################################################################################
        
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