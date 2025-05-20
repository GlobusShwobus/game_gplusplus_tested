#include "BasicComponents.h"

NPCState::State NPCState::getState()const {
	return currentState;
}
NPCState::Facing NPCState::getFacing()const {
	return currentFacing;
}
void NPCState::setState(const std::pair<NPCState::State, NPCState::Facing>* const state) {
	if (!(state->first == State::noChange)) {
		currentState = state->first;
	}
	if (!(state->second == Facing::noChange)) {
		currentFacing = state->second;
	}
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

void AnimationController::update(AnimID id) {
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
	bool exists = false;
	for (const AnimationReel& each : *clips) {
		if (each.id == id) {
			currentReel = &each;//praying this sets address and not some other voodoo
			frameIndex = 0;
			exists = true;
		}
	}

	if (!exists) {
		printf("\nEntered invalid animation for this entity: %d\n", id);
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

