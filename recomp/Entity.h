#pragma once

#include "Components.h"
#include "vec2.h"

enum class EntityType;

class Entity {

	friend class EntityManager;

	SDL_FRect bb;
	SDL_Texture* texture = nullptr;
	int ID = 0;
	EntityType type;
	bool isActive = true;

	Entity(const int id, EntityType Type, const SDL_FRect& box, SDL_Texture* textPtr) :texture(textPtr), bb(box), ID(id), type(Type) {}
public:

	SDL_Texture* GetTexture()const;
	SDL_FRect* GetBB();
	EntityType GetType()const;
	int GetID()const;
	void Kill();
	bool IsActive()const;
	vec2f GetPosition()const;

};


enum class EntityType {
	player, enemy
};
