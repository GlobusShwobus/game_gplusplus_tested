#include "EntityFactory.h"
#include "RenderWindow.h"
#include "Grid.h"
#include "Player.h"

#include "MyUtils.h"


static constexpr int logicalRendererWidthInitial = 1280;
static constexpr int logicalRendererHeightInitial = 720;

int main() {

    
    //configs
    nlohmann::json* metaConfig      = nullptr;
    nlohmann::json* animationConfig = nullptr;

    try {
        metaConfig = MyUtils::initJSON("Config.json");
        animationConfig = MyUtils::initJSON("animationClips.json");
    }
    catch (const std::exception& excpt) {
        printf(excpt.what());
        if (metaConfig) {
            delete metaConfig;
        }
        if (animationConfig) {
            delete animationConfig;
        }
        return -1;
    }
    //initalize SDL and RenderWIndow
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("\nCRASH:: SDL_init Failure");
        return -1;
    }

    RenderWindow window(metaConfig);

    window.setLogicalRenderingSize(logicalRendererWidthInitial, logicalRendererHeightInitial);


    //initialize TextureManager
    EntityFactory textureManager;

    try {
        textureManager.bootUpSprites(window.getRenderer(), metaConfig);
        textureManager.bootUpAnimations(animationConfig);
    }
    catch (const std::exception& e) {
        printf("\nEntityManager BootUp failure: %s\n", e.what());
        std::exit(EXIT_FAILURE);
    }


    //TEST (later scene?) worldmap
    Sprite worldMap = textureManager.createSprite(SpriteID::world_map);

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


    window.~RenderWindow();
    SDL_Quit();

    delete metaConfig;
    delete animationConfig;

    return 0;
}
