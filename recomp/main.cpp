#include <fstream>
#include "JSON_Config.h"
#include "Texture_Manager.h"
#include "Entity_Manager.h"
#include "RenderWindow.h"
#include "Grid.h"

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
        MessageBoxW(nullptr, L"SDL init fail", L"error init", MB_OK | MB_ICONERROR);
        return -1;
    }

    RenderWindow window(wParams.title.c_str(), wParams.width, wParams.height);

    if (!window.good()) {
        MessageBoxW(nullptr, L"Window failed init", L"error init", MB_OK | MB_ICONERROR);
        return -1;
    }

    //initialize TextureManager

    TextureManager textman;

    for (const auto& paths : FileManager::getPNGs("../Textures")) {
        textman.cacheTexture(paths.stem().string(), window.loadNewTexture(paths.string().c_str()));
    }

    //initialize EntityManager, and grid

    EntityManager entman;

    auto TESTMAP = textman.GetTexture("worldmap");
    InstancedGrid mapgrid(TESTMAP->w, TESTMAP->h);



    //main game loop

    bool gameRunning = true;
    SDL_Event event;

    while (gameRunning) {

        entman.Update();


        SDL_PollEvent(&event);

        if (event.type == SDL_EventType::SDL_EVENT_QUIT) {
            gameRunning = false;
        }
        else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {

                for (auto& ent : entman.GetEntities()) {

                    ent->Kill();

                }

            }
            else if (event.button.button == SDL_BUTTON_RIGHT) {

                entman.AddEntity(*config.Get("Player"), EntityType::player, textman);

            }
        }
        //move~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        else if (event.type == SDL_EVENT_KEY_DOWN) {
            for (Entity* entity: entman.GetEntities()) {//fuck it it's just test code
                auto* bb = entity->GetBounds();
                switch (event.key.scancode) {
                case SDL_SCANCODE_W:
                    printf("Up is pressed\n");
                    if (!mapgrid.isNextFilled(bb->x, bb->y, up)) {
                        mapgrid.setNextTile(up, bb);
                    }
                    break;
                case SDL_SCANCODE_A:
                    printf("left is pressed\n");
                    if (!mapgrid.isNextFilled(bb->x, bb->y, left)) {
                        mapgrid.setNextTile(left, bb);
                    }
                    break;
                case SDL_SCANCODE_S:
                    printf("down is pressed\n");
                    if (!mapgrid.isNextFilled(bb->x, bb->y, down)) {
                        mapgrid.setNextTile(down, bb);
                    }
                    break;
                case SDL_SCANCODE_D:
                    printf("right is pressed\n");
                    if (!mapgrid.isNextFilled(bb->x, bb->y, right)) {
                        mapgrid.setNextTile(right, bb);
                    }
                    break;
                default://fuck off
                    break;
                }
            }
        }
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        window.clear();


        window.render(TESTMAP, nullptr);//idk wtf to do with this atm, i guess whenever i am ready to make rendering funcs more detailed, also order of drawing matters

        for (Entity* entity : entman.GetEntities()) {
            window.render(entity->GetTexture(), entity->GetBounds());
        }


        int cont_size = entman.GetEntities().size();

        //printf("grid size: %d x %d == %d\n", mapgrid.getW(), mapgrid.getH(), mapgrid.getW()* mapgrid.getH()); //testing to make sure the grid is correct size
        //printf("\n%s%d", "Entity size: ", cont_size);  //testing how many entities on exist


        window.display();

    }


    window.~RenderWindow();
    SDL_Quit();

    return 0;
}
