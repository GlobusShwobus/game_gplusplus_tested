#include "BasicComponents.h"

void EntityState::deactivate() {
	isActive = false;
}
bool EntityState::isActivated()const {
	return isActive;
}
void EntityState::changeAction(EntityAction action) {
	if (this->action != action) {
		this->action = action;
	}
}
void EntityState::changeDirection(Direction direction) {
	if (this->direction != direction) {
		this->direction = direction;
		eventFlags |= EntityEvents::directionChange;
	}
}
void EntityState::setEvent(const EntityEvents events) {
	eventFlags |= events;
}
void EntityState::flushEvents() {
	eventFlags = 0;
}
int EntityState::getEvents()const {
	return eventFlags;
}
bool EntityState::containsEvent(EntityEvents event)const {
	return eventFlags & event;
}
Direction EntityState::getDirection()const {
	return direction;
}
EntityAction EntityState::getAction()const {
	return action;
}



void AnimationController::moveFrame() {
	if (!currentReel->isLooping && frameIndex >= currentReel->frameCount) return;//not looping so nothing to update
	frameTimer++;

	//if we haven't reached the treshhold to update frame, leave
	if (frameTimer < frameDelay)return;

	//entered next frame
	frameTimer = 0;
	frameIndex++;
	//reset it if it is looping type and reached the end
	if (currentReel->isLooping && frameIndex >= currentReel->frameCount) {
		frameIndex = 0;
	}
}
void AnimationController::setNewReel(AnimID id) {
	if (clips->contains(id) && currentID != id) {
		currentID = id;
		currentReel = &clips->at(id);
		frameIndex = 0;
	}
}
void AnimationController::applySourceFromFrame(SDL_FRect& rect)const {
	rect.x = currentReel->beginX + (rect.w * frameIndex);
	rect.y = currentReel->beginY;
}

