#include "Texture_Manager.h"
#include "RenderWindow.h"
#include "Grid.h"
#include "Player.h"

#include "MyUtils.h"
using namespace MyUtils;

#include "TESTS.h"


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

    if (!window.good()) {
        printf("\n\nCRASH:: Window init Failure");
        return -1;
    }

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
    Player player = Player(textureManager.GetTexture("player_ver2"));


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

        window.clear();

        window.render(worldMap);
        window.render(player.sprite);
        
       
        window.display();
    }


    window.~RenderWindow();
    SDL_Quit();

    return 0;
}
