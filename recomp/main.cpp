#include <fstream>
#include "JSON_Config.h"
#include "Texture_Manager.h"
#include "Entity_Manager.h"
#include "RenderWindow.h"
#include "Grid.h"

/*
spawn player with right click
despawn player with left click

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
    FileManager fileman;
    JSONConfig config;

    config.Init(fileman.GetEntityConfig("Config.json"));

    if (!config.Good()) {
        MessageBoxW(nullptr, L"Config failed init", L"error init", MB_OK | MB_ICONERROR);
        return -1;
    }

    WindowInit wParams(config.Get());

    //initalize SDL and RenderWIndow
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        MessageBoxW(nullptr, L"SDL init fail", L"error init", MB_OK | MB_ICONERROR);
        return -1;
    }

    RenderWindow window(wParams.title.c_str(), wParams.width, wParams.height);

    if (!window.Good()) {
        MessageBoxW(nullptr, L"Window failed init", L"error init", MB_OK | MB_ICONERROR);
        return -1;
    }

    //initialize TextureManager

    TextureManager textman;

    for (const auto& paths : fileman.GetTextureFolderContents("../Textures")) {

        SDL_Texture* passownershiptoidkdoireallycareifsomeonereadsthis = window.UniqueTextureLoad(paths.string().c_str());

        textman.InitializeTexture(paths.stem().string(), passownershiptoidkdoireallycareifsomeonereadsthis);


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
            for (auto& ent : entman.GetEntities()) {//fuck it it's just test code
                auto* bb = ent->GetBB();
                switch (event.key.scancode) {
                case SDL_SCANCODE_W:
                    printf("Up is pressed\n");
                    if (!mapgrid.isNextFilled(bb->x, bb->y, up)) {
                        mapgrid.SetNextTile(up, bb);
                    }
                    break;
                case SDL_SCANCODE_A:
                    printf("left is pressed\n");
                    if (!mapgrid.isNextFilled(bb->x, bb->y, left)) {
                        mapgrid.SetNextTile(left, bb);
                    }
                    break;
                case SDL_SCANCODE_S:
                    printf("down is pressed\n");
                    if (!mapgrid.isNextFilled(bb->x, bb->y, down)) {
                        mapgrid.SetNextTile(down, bb);
                    }
                    break;
                case SDL_SCANCODE_D:
                    printf("right is pressed\n");
                    if (!mapgrid.isNextFilled(bb->x, bb->y, right)) {
                        mapgrid.SetNextTile(right, bb);
                    }
                    break;
                default://fuck off
                    break;
                }
            }
        }
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        window.Clear();


        window.Render(TESTMAP, nullptr);//idk wtf to do with this atm, i guess whenever i am ready to make rendering funcs more detailed, also order of drawing matters

        for (auto& ent : entman.GetEntities()) {
            window.Render(ent->GetTexture(), ent->GetBB());
        }


        int cont_size = entman.GetEntities().size();

        //printf("grid size: %d x %d == %d\n", mapgrid.getW(), mapgrid.getH(), mapgrid.getW()* mapgrid.getH()); //testing to make sure the grid is correct size
        //printf("\n%s%d", "Entity size: ", cont_size);  //testing how many entities on exist


        window.Display();

    }


    window.~RenderWindow();
    SDL_Quit();

    return 0;
}
