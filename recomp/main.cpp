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

    //TEST (later scene?) worldmap
    //Sprite worldMap = textureManager.createSprite(SpriteID::world_map);

    //grid
    Grid grid = Grid(worldMap.getTexture()->w, worldMap.getTexture()->h);
    //player
    Player player = Player(textureManager.createSprite(SpriteID::player_sheet) , SDL_FPoint{ logicalRendererWidthInitial, logicalRendererHeightInitial });


    bool gameRunning = true;
    SDL_Event event;

    while (gameRunning) {

        window.updateBegin();


        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                gameRunning = false;
            }

            //EVENTS
            player.movement.movementUpdate();//reads event key press moves
            //#################################################################################
        }

        //MOVEMENT
        SDL_FRect tPos = MyUtils::getNewPosition(player.sprite.getDestination(), player.movement.getCurrentMove(), player.speed);
        MyUtils::updatePosition(grid, tPos, player.sprite.getDestination());
        //#################################################################################

        //CAMERA
        player.camera.setFocus(player.sprite.getDestination());
        player.camera.setClamp(worldMap.getSource());
        //#################################################################################

        //ANIMATION
        const ClipID clip = MyUtils::getClipBasedOnMovement(player.movement);
        player.sprite.play(clip);
        //#################################################################################

        MyUtils::Rendering::renderBasic(window.getRenderer(), worldMap, player.camera);
        MyUtils::Rendering::renderBasic(window.getRenderer(), player.sprite, player.camera);


        window.updateEnd();
    }


    window.~Window();
    SDL_Quit();


    //OR GET RID OF THEM AFTER USING THEM, FOOD FOR THOUGHT LATER
    delete entityConfig;
    delete stageConfig;
    delete windowConfig;

    return 0;
}
