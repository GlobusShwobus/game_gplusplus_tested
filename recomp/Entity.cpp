#include "Entity.h"

SDL_Texture* Entity::GetTexture()const {
	return texture;
}
uint64_t Entity::GetDescription()const {
	return description;
}
void Entity::AddDescription(const int addDescription) {
	description |= addDescription;
}
void Entity::RemoveDescription(const int removeDescription) {
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

void EntityManager::AddEntity(uint64_t description, SDL_Texture* pTexture) {
	temporaryEntities.push_back(Entity{ description, pTexture });
}
