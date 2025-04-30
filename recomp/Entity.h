#pragma once

//#include "Sprite.h"
//#include <vector>

/*
*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*
* DEPRICATED UNTIL ACTUAL WORK WITH OTHER ENTITIES IS DONE
* 
*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/





/*
enum EntityDescriptor {
	ed_isActive = 1 << 0,
	ed_Kill     = 1 << 1,
	ed_canMove  = 1 << 2 
};
typedef uint64_t Descriptor;

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
*/
/*
const Sprite& Entity::GetSprite()const {
	return sprite;
}
uint64_t Entity::GetDescription()const {
	return description;
}
void Entity::AddDescription(const Descriptor addDescription) {
	description |= addDescription;
}
void Entity::RemoveDescription(const Descriptor removeDescription) {
	description &= ~removeDescription;
}


void EntityManager::Update()
{
	for (const Entity& entity : temporaryEntities) {
		entities.push_back(entity);
	}
	temporaryEntities.clear();
	RemoveInactive();
}
void EntityManager::RemoveInactive()
{
	entities.erase(std::remove_if(entities.begin(), entities.end(), [](const Entity& entity) {
		return !(entity.GetDescription() & EntityDescriptor::ed_isActive);
		}), entities.end());
}

void EntityManager::AddEntity(Descriptor description, SDL_Texture* pTexture) {
	temporaryEntities.push_back(Entity{ description, pTexture });
}
std::vector<Entity>& EntityManager::GetEntities() {
	return entities;
}

*/


