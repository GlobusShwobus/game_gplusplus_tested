#include "BasicComponents.h"

void Movement::movementUpdate() {
	const auto* keystate = SDL_GetKeyboardState(nullptr);

	bool w = keystate[SDL_SCANCODE_W];
	bool a = keystate[SDL_SCANCODE_A];
	bool s = keystate[SDL_SCANCODE_S];
	bool d = keystate[SDL_SCANCODE_D];

	if (w && a) {
		movementStatus = MS::MOVE_UP_LEFT;
		lastMove = MS::MOVE_UP_LEFT;
	}
	else if (w && d) {
		movementStatus = MS::MOVE_UP_RIGHT;
		lastMove = MS::MOVE_UP_RIGHT;
	}
	else if (s && a) {
		movementStatus = MS::MOVE_DOWN_LEFT;
		lastMove = MS::MOVE_DOWN_LEFT;
	}
	else if (s && d) {
		movementStatus = MS::MOVE_DOWN_RIGHT;
		lastMove = MS::MOVE_DOWN_RIGHT;
	}
	else if (w) {
		movementStatus = MS::MOVE_UP;
		lastMove = MS::MOVE_UP;
	}
	else if (s) {
		movementStatus = MS::MOVE_DOWN;
		lastMove = MS::MOVE_DOWN;
	}
	else if (a) {
		movementStatus = MS::MOVE_LEFT;
		lastMove = MS::MOVE_LEFT;
	}
	else if (d) {
		movementStatus = MS::MOVE_RIGHT;
		lastMove = MS::MOVE_RIGHT;
	}
	else {
		movementStatus = MS::MOVEMENT_STATUS_NOTHING;
	}
}
MovementStatus Movement::getCurrentMove()const {
	return movementStatus;
}
MovementStatus Movement::getLastMove()const {
	return lastMove;
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

