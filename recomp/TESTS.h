#pragma once

#include "Window.h"
#include "Grid.h"
#include "EntityFactory.h"
#include <random>

namespace TESTS {


    void SetNoWalkingZone(Grid& grid);
    void TestNoWalkingZone(SDL_Renderer* renderer);

    class RandomNumberGenerator {//actually neat, need to put in components?
        std::random_device rd;
        std::unique_ptr<std::mt19937> rng;
    public:
        RandomNumberGenerator() :rng(std::make_unique<std::mt19937>(rd())) {}
        int getRand(int min, int max) {
            return std::uniform_int_distribution<int>(min, max)(*rng);
        }
    };

    class ENTITY_MANAGER_TEST {

        std::vector<EnemyID> requestAddition;
        std::vector<EnemyBasic*> enemies;

    public:

        void addEnemy(const char* enemy) {
            EnemyID id = HASH(enemy);
            requestAddition.push_back(id);
        }
        void removeEnemy() {
            
            enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                [](EnemyBasic* enemy) {
                    if (!enemy->isActivated) {
                        delete enemy;
                        return true;
                    }
                    else {
                        return false;
                    }
                }),
                enemies.end()
            );

            /*
            //IF I NEED A MORE CONTROLLED APPROACH LIKE UPDATE 
           
            for (EnemyBasic* enemy : enemies) {
                if (!enemy->isActivated) {
                    delete enemy;
                    enemy = nullptr;
                }
            }

            enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](EnemyBasic* enemy) {
                return enemy == nullptr;
                }), enemies.end());
        */
        }
        
        //optionalCount is for handling addition inbetween frames, in cases we have too many requests, i can throttle it stay within 60 FPS
        void update(const EntityFactory& factory, Uint32 optionalCount = 0) {
            int itFor = requestAddition.size();
            if (optionalCount > 0 && optionalCount < itFor) {
                itFor = optionalCount;
            }
        
            int counter = 0;
            for (EnemyID id : requestAddition) {
                if (itFor == counter) {
                    break;
                }
                ++counter;

                EnemyBasic* entry = factory.createEnemy(id);
                if (entry != nullptr) {
                    enemies.push_back(entry);
                }

            }
            requestAddition.clear();
            
            printf("entities: %d\n", enemies.size());
            printf("requests: %d\n", requestAddition.size());
        }

        const std::vector<EnemyBasic*>& getEnemies()const {
            return enemies;
        }
    };

    void ASK_10_ENEMIES(ENTITY_MANAGER_TEST& entman);
    void GIVE_RANDOM_POSITIONS_TO_ENEMIES(ENTITY_MANAGER_TEST& entman, RandomNumberGenerator& lolw);
    void CLEAR_ALL_ENTITIES(ENTITY_MANAGER_TEST& entman);

}