#include "JSON_Config.h"
#include "Texture_Manager.h"
#include "RenderWindow.h"
#include "Grid.h"
#include "Entity.h"

/*

SMALL REWRITE: use proper c++ meaning pointers
JSON get specific am not happy with
grid is i think fucked because it does not adjust current pos/and current pos pixel, with grid and itstead does calcualtions raw
fix movement
remove junk test code and encapsulate behavior
figure a way to make map manager
*/

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

    //initialize EntityManager, and grid

    EntityManager* entman = new EntityManager();


    const nlohmann::json* mapJson = config.Get("Worldmap");
    const float w = (*mapJson)["width"].get<int>();
    const float h = (*mapJson)["height"].get<int>();
    Sprite TESTMAP = Sprite(textman.GetTexture("worldmap"), { 0,0,w,h }, { 0,0,w,h });


    InstancedGrid mapgrid(TESTMAP.source.w, TESTMAP.source.h);

    //main game loop

    bool gameRunning = true;
    SDL_Event event;

    while (gameRunning) {

        entman->Update();


        SDL_PollEvent(&event);

        if (event.type == SDL_EventType::SDL_EVENT_QUIT) {
            gameRunning = false;
        }
        else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                for (auto& entity:entman->GetEntities()) {

                    entity.RemoveDescription(ed_isActive);
                }

            }
            else if (event.button.button == SDL_BUTTON_RIGHT) {
                //
                const nlohmann::json* playerJson = config.Get("Player");
                std::string texName = (*playerJson)["texture"].get<std::string>();
                SDL_Texture* texture = textman.GetTexture(texName);
                //
                entman->AddEntity(ed_isActive | ed_canMove, texture);
            }
        }
        //move~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        else if (event.type == SDL_EVENT_KEY_DOWN) {
            for (auto& entity : entman->GetEntities()) {
                if ((entity.GetDescription() & ed_canMove)) {
                    
                   /*
                    switch (event.key.scancode) {
                    case SDL_SCANCODE_W:
                        printf("Up is pressed\n");
                        if (!mapgrid.isNextFilled(entity.GetSprite().source.w, entity.GetSprite().source.h, up)) {
                            mapgrid.setNextTile(up, entity.GetSprite().source);
                        }
                        break;
                    case SDL_SCANCODE_A:
                        printf("left is pressed\n");
                        if (!mapgrid.isNextFilled(entity.GetSprite().source.w, entity.GetSprite().source.h, left)) {
                            mapgrid.setNextTile(left, bb);
                        }
                        break;
                    case SDL_SCANCODE_S:
                        printf("down is pressed\n");
                        if (!mapgrid.isNextFilled(entity.GetSprite().source.w, entity.GetSprite().source.h, down)) {
                            mapgrid.setNextTile(down, bb);
                        }
                        break;
                    case SDL_SCANCODE_D:
                        printf("right is pressed\n");
                        if (!mapgrid.isNextFilled(entity.GetSprite().source.w, entity.GetSprite().source.h, right)) {
                            mapgrid.setNextTile(right, bb);
                        }
                        break;
                    default://fuck off
                        break;
                    }
                    */
                }

            }
        }
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        window.clear();


        window.render(TESTMAP);//idk wtf to do with this atm, i guess whenever i am ready to make rendering funcs more detailed, also order of drawing matters

        for (auto& entity : entman->GetEntities()) {

            window.render(entity.GetSprite());
        }


        int cont_size = entman->GetEntities().size();

        printf("\grid size: %d x %d == %d", mapgrid.getW(), mapgrid.getH(), mapgrid.getW()* mapgrid.getH()); //testing to make sure the grid is correct size
        printf("\n%s%d", "Entity size: ", cont_size);  //testing how many entities on exist


        window.display();

    }


    window.~RenderWindow();
    SDL_Quit();
    delete entman;

    return 0;
}
