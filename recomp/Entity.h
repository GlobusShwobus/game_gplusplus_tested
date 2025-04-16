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
typedef uint64_t Descriptor;

struct Sprite {
	SDL_Texture* texture = nullptr;
	SDL_FRect source = { 0,0,32,32 };
	SDL_FRect destination = { 0,0,32,32 };

	Sprite(SDL_Texture* pTexture) :texture(pTexture) {}
	Sprite(SDL_Texture* pTexture, SDL_FRect src, SDL_FRect dest) :texture(pTexture), source(src), destination(dest) {}
};


class Entity {

	friend class EntityManager;

	Sprite sprite;
	uint64_t description = 0;

	Entity(uint64_t description, Sprite textPtr) :sprite(textPtr), description(description) {}

public:

	const Sprite& GetSprite()const;
	uint64_t GetDescription()const;
	void AddDescription(const Descriptor addDescription);
	void RemoveDescription(const Descriptor removeDescription);
};

class EntityManager {

	std::vector<Entity> temporaryEntities;
	std::vector<Entity> entities;

	void RemoveInactive();

public:

	EntityManager() {}

	void Update();

	void AddEntity(Descriptor description, SDL_Texture* pTexture);

	std::vector<Entity>& GetEntities();

};


