#include "Entity.h"


EntityType Entity::GetType()const {
	return type;
}
int Entity::GetID()const {
	return ID;
}
void Entity::Kill() {
	isActive = false;
}
bool Entity::IsActive()const {
	return isActive;
}
vec2f Entity::GetPosition()const {
	return { bb.x, bb.y };
}
SDL_Texture* Entity::GetTexture()const {
	return texture;
}
SDL_FRect* Entity::GetBB() {
	return &bb;
}