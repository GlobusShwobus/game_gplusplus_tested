#include "BasicComponents.h"

void NPCState::setState(const State state) {
	if (state != currentState) {
		currentState = state;
		wasChange = true;
	}
}
void NPCState::setDirection(const Direction facing) {
	if (facing != currentDirection) {
		currentDirection = facing;
		wasChange = true;
	}
}
NPCState::State NPCState::getState()const {
	return currentState;
}
NPCState::Direction NPCState::getDirection()const {
	return currentDirection;
}
bool NPCState::didChangeOccur()const {
	return wasChange;
}
void NPCState::handeledChange() {
	wasChange = false;
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
	if (clips && clips->contains(id) && currentID != id) {
		currentID = id;
		currentReel = &clips->at(id);
		frameIndex = 0;
	}
}
void AnimationController::applySourceFromFrame(SDL_FRect& rect)const {
	rect.x = currentReel->beginX + (rect.w * frameIndex);
	rect.y = currentReel->beginY;
}

