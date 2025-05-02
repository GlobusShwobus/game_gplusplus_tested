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
    TextureManager textureManager;
    for (const auto& paths : Files::getPNGs("../Textures")) {
        textureManager.cacheTexture(paths.stem().string(), window.loadNewTexture(paths.string().c_str()));
    }

    //garbanzo for now, plz fix

    const nlohmann::json* mapJson = config.Get("Worldmap");
    const float w = (*mapJson)["width"].get<int>();
    const float h = (*mapJson)["height"].get<int>();
    Sprite worldMap = Sprite(textureManager.GetTexture("worldmap"), { 0,0,w,h }, { 0,0,w,h });

    //grid
    Grid grid = Grid(w, h);
    //player
    Player player = Player(textureManager.GetTexture("player_ver2"), SDL_FPoint{ logicalRendererWidthInitial, logicalRendererHeightInitial });

    bool gameRunning = true;
    SDL_Event event;

    while (gameRunning) {

        SDL_PollEvent(&event);

        if (event.type == SDL_EventType::SDL_EVENT_QUIT) {
            gameRunning = false;
        }


        //EVENTS
        player.keyboard.queueMove(&event);//reads event key press moves
        //-------------------------

        //MOVEMENT
        SDL_FRect tPos = player.keyboard.getNewPosition(&player.sprite.destination, player.speed);
        MyUtils::updatePosition(grid, tPos, &player.sprite.destination);
        //--------------------------

        //CAMERA
        player.camera.setFocus(&player.sprite.destination);
        player.camera.setClamp(&worldMap.source);
        //--------------------------


        window.clear();
        Rendering::renderBasic(window.getRenderer(), worldMap, player.camera);
        Rendering::renderBasic(window.getRenderer(), player.sprite, player.camera);

        window.display();
    }


    window.~RenderWindow();
    SDL_Quit();

    return 0;
}
