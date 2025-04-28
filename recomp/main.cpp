#include "JSON_Config.h"
#include "Texture_Manager.h"
#include "RenderWindow.h"
#include "Grid.h"
#include "Entity.h"
#include "Player.h"


#include "TESTS.h"

struct WindowInit {

    std::string title;
    unsigned int width = 0;
    unsigned int height = 0;

    WindowInit(const nlohmann::json& json) {

        auto& wStuff = json["Window"];

        title = wStuff["game_name"];
        width = wStuff["width"];
        height = wStuff["height"];
    }
};


int main() {

    //initialize config and file manager, (and window but probably should move window at some point)

    JSONConfig config(FileManager::getNewConfig("Config.json"));//will throw if nullptr, but does not check for any specific json recieved, meaning it can still fuck up down the line

    WindowInit wParams(config.Get());

    //initalize SDL and RenderWIndow
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("\n\nCRASH:: SDL_init Failure");
        return -1;
    }

    RenderWindow window(wParams.title.c_str(), wParams.width, wParams.height);

    if (!window.good()) {
        printf("\n\nCRASH:: Window init Failure");
        return -1;
    }

    //initialize TextureManager

    TextureManager textman;

    for (const auto& paths : FileManager::getPNGs("../Textures")) {
        textman.cacheTexture(paths.stem().string(), window.loadNewTexture(paths.string().c_str()));
    }

    //garbanzo for now,

    const nlohmann::json* mapJson = config.Get("Worldmap");
    const float w = (*mapJson)["width"].get<int>();
    const float h = (*mapJson)["height"].get<int>();
    Sprite TESTMAP = Sprite(textman.GetTexture("worldmap"), { 0,0,w,h }, { 0,0,w,h });

    //grid
    Grid grid = Grid(w, h);

    //player

    Player player = Player(textman.GetTexture("player_ver2"));




    //****************

    //TESTSCODE:

    TESTS::SetNoWalkingZone(grid);


    //****************


    //main game loop

    bool gameRunning = true;
    SDL_Event event;

    while (gameRunning) {

        SDL_PollEvent(&event);

        if (event.type == SDL_EventType::SDL_EVENT_QUIT) {
            gameRunning = false;
        }

        player.keyboard.queueMove(&event);//reads event key press moves


        vec2f playerMoveTest = { player.sprite.destination.x, player.sprite.destination.y };

        player.keyboard.processMove(grid, playerMoveTest, player.speed);
        
        player.sprite.setDestination(playerMoveTest);


        window.clear();



        window.render(TESTMAP);//idk wtf to do with this atm, i guess whenever i am ready to make rendering funcs more detailed, also order of drawing matters
        window.render(player.sprite);
        

        TESTS::TestNoWalkingZone(window.getRenderer());
        
        
        window.display();

    }


    window.~RenderWindow();
    SDL_Quit();

    return 0;
}
