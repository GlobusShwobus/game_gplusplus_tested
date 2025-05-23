#include "BasicComponents.h"

void NPCState::setState(const State state) {
	if (state != currentState) {
		currentState = state;
		wasChange = true;
	}
}
void NPCState::setFacing(const Facing facing) {
	if (facing != currentFacing) {
		currentFacing = facing;
		wasChange = true;
	}
}

NPCState::State NPCState::getState()const {
	return currentState;
}
NPCState::Facing NPCState::getFacing()const {
	return currentFacing;
}

bool NPCState::didChangeOccur()const {
	return wasChange;
}
void NPCState::handeledChange() {
	wasChange = false;
}

SDL_Texture* Sprite::getTexture() {
	return texture;
}
SDL_FRect* Sprite::getSource() {
	return &source;
}
SDL_FRect* Sprite::getDestination() {
	return &destination;
}

void AnimationController::update() {
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
const SDL_FRect AnimationController::getCurrentFrame()const {

	SDL_FRect frame{ 0,0,0,0 };

	frame.x = currentReel->initialFrame.x + (currentReel->initialFrame.w * frameIndex);
	frame.y = currentReel->initialFrame.y;
	frame.w = currentReel->initialFrame.w;
	frame.h = currentReel->initialFrame.h;
	return frame;
}

