#pragma once

#define PI 3.14159265
#define NOMINMAX

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include <vector>

enum EntityDescriptor {
	ed_isActive = 1 << 0,
	ed_Kill   = 1 << 1,
	ed_canMove  = 1 << 2 
};
 
class Entity {

	friend class EntityManager;

	SDL_Texture* texture = nullptr;
	uint64_t description = 0;

	Entity(uint64_t description, SDL_Texture* textPtr) :texture(textPtr), description(description) {}

public:

	SDL_Texture* GetTexture()const;
	uint64_t GetDescription()const;
	void AddDescription(const int addDescription);
	void RemoveDescription(const int removeDescription);
};

class EntityManager {

	std::vector<Entity> temporaryEntities;
	std::vector<Entity> entities;

	void RemoveInactive();

public:

	EntityManager() {}

	void Update();

	void AddEntity(uint64_t description, SDL_Texture* pTexture);

	std::vector<Entity>& GetEntities() {
		return entities;
	}

};


