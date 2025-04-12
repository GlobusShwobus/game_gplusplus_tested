#pragma once

#include "Components.h"
#include "vec2.h"


enum class EntityType {
	player, enemy, NONE
};


class Entity {

	friend class EntityManager;

	SDL_FRect boundingBox;
	SDL_Texture* texture = nullptr;
	int ID = 0;
	EntityType type = EntityType::NONE;
	bool isActive = false;

	Entity(const int id, EntityType Type, const SDL_FRect& box, SDL_Texture* textPtr) :texture(textPtr), boundingBox(box), ID(id), type(Type) {}

	void Enable();
public:

	int GetID()const;
	EntityType GetType()const;
	bool IsActive()const;


	SDL_Texture* GetTexture()const;
	SDL_FRect* GetBounds();


	void Kill();

};


