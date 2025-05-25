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
	if (frameTimer < currentReel->frameDelay)return;

	//entered next frame
	frameTimer = 0;
	frameIndex++;
	//reset it if it is looping type and reached the end
	if (currentReel->isLooping && frameIndex >= currentReel->frameCount) {
		frameIndex = 0;
	}
}
void AnimationController::setNewReel(AnimID id) {
	for (const AnimationReel& each : *clips) {
		if (each.id == id && currentReel->id != id) {//if it even exists and if it isn't the one already in use
			currentReel = &each;//praying this sets address and not some other voodoo
			frameIndex = 0;
		}
	}
}
void AnimationController::applySourceFromFrame(SDL_FRect* const rect)const {
	rect->x = currentReel->initialFrame.x + (currentReel->initialFrame.w * frameIndex);
	rect->y = currentReel->initialFrame.y;
	rect->w = currentReel->initialFrame.w;
	rect->h = currentReel->initialFrame.h;
}

