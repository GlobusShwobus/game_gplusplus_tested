#include "Texture_Manager.h"
#include "RenderWindow.h"
#include "Grid.h"
#include "Player.h"

#include "MyUtils.h"
using namespace MyUtils;

#include "TESTS.h"


static constexpr int logicalRendererWidthInitial = 1280;
static constexpr int logicalRendererHeightInitial = 720;

int main() {

    
    //config
    Config::jConfig config("Config.json");

    if (!config.isValid()) {
        printf("\nERROR:: Config as nullptr (not existing in directory or wrong name)\n");
        return -1;
    }


    //initalize SDL and RenderWIndow
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("\nCRASH:: SDL_init Failure");
        return -1;
    }

    RenderWindow window(config.Get());

    window.setLogicalRenderingSize(logicalRendererWidthInitial, logicalRendererHeightInitial);


    //initialize TextureManager
    TextureManager textureManager(window.getRenderer(), "../Textures");


    //TEST (later scene?) worldmap
    Sprite worldMap = textureManager.createSprite(config.Get("Worldmap"));

    //grid
    Grid grid = Grid(worldMap.getTexture()->w, worldMap.getTexture()->h);
    //player
    Player player = Player(textureManager.createSprite(config.Get("Player")) , SDL_FPoint{ logicalRendererWidthInitial, logicalRendererHeightInitial });

    bool gameRunning = true;
    SDL_Event event;

    while (gameRunning) {

        window.updateBegin();


        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                gameRunning = false;
            }

            //EVENTS
            player.movement.moveBegin(&event);//reads event key press moves
            //-------------------------

        }

        //MOVEMENT
        SDL_FRect tPos = MyUtils::getNewPosition(player.sprite.getDestination(), player.movement.getMovementStatus(), player.speed);
        MyUtils::updatePosition(grid, tPos, player.sprite.getDestination());
        //--------------------------

        //CAMERA
        player.camera.setFocus(player.sprite.getDestination());
        player.camera.setClamp(worldMap.getSource());
        //--------------------------


        Rendering::renderBasic(window.getRenderer(), worldMap, player.camera);
        Rendering::renderBasic(window.getRenderer(), player.sprite, player.camera);

        player.movement.moveEnd();
        window.updateEnd();
    }


    window.~RenderWindow();
    SDL_Quit();

    return 0;
}
