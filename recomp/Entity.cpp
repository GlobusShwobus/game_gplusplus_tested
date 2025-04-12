#include "Entity.h"


int Entity::GetID()const {
	return ID;
}
EntityType Entity::GetType()const {
	return type;
}
bool Entity::IsActive()const {
	return isActive;
}

void Entity::Enable() {
	isActive = true;
}
void Entity::Kill() {
	isActive = false;
}


SDL_Texture* Entity::GetTexture()const {
	return texture;
}
SDL_FRect* Entity::GetBounds() {
	return &boundingBox;
}