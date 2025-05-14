#include "Player.h"

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
void Camera::setFocus(const SDL_FRect* const playerPos) {
	//camera center at player's center
	center.x = playerPos->x + (playerPos->w / 2);
	center.y = playerPos->y + (playerPos->h / 2);
}
void Camera::setClamp(const SDL_FRect* const worldMap) {
	if (center.x - radiusWidth < 0) { center.x = radiusWidth; }//left edge
	if (center.y - radiusHeight < 0) { center.y = radiusHeight; }//top edge
	if (center.x + radiusWidth > worldMap->w) { center.x = worldMap->w - radiusWidth; }//right edge
	if (center.y + radiusHeight > worldMap->h) { center.y = worldMap->h - radiusHeight; }//bottom edge
}
const SDL_FPoint Camera::getCenter()const {
	return center;
}
SDL_FRect Camera::toCameraSpace(const SDL_FRect* const entity)const {
	SDL_FRect screenPos{ 0,0,0,0 };

	screenPos.x = entity->x - (center.x - radiusWidth);
	screenPos.y = entity->y - (center.y - radiusHeight);

	screenPos.w = entity->w;
	screenPos.h = entity->h;

	return screenPos;
}

void Sprite::play(const ClipID clipID) {
	//if this status, then nothing to do here
	if (clipID == ClipID::none || !clips) return;

	const auto& clipIt = clips->find(clipID);

	//maybe entered a clip which is not appropriate to the texture, then gtfo
	if (clipIt == clips->end()) return;

	const AnimationClip& clip = clipIt->second;

	//if clips changed, reset local vars related to frames
	if (clipID != previousClip) {
		previousClip = clipID;
		frameIndex = 0;
		clipTimer = 0;

		source.x = clip.x;
		source.y = clip.y;
		source.w = clip.w;
		source.h = clip.h;
		return;
	}

	//if frame is of looping type and we reached the end of the animation, don't repeat it
	if (!clip.isLooping && frameIndex >= clip.frameCount) return;


	clipTimer++;
	//if we haven't reached the treshhold to update frame, leave
	if (clipTimer < clip.frameDelay)return;

	clipTimer = 0;

	frameIndex++;
	//reset it if it is looping type and reached the end
	if (clip.isLooping && frameIndex >= clip.frameCount) {
		frameIndex = 0;
	}
	//do the actual magic
	source.x = clip.x + (clip.w * frameIndex);
	source.y = clip.y;
	source.w = clip.w;
	source.h = clip.h;
}
void Sprite::setClips(std::map<ClipID, AnimationClip>* clips) {
	this->clips = clips;
}
bool Sprite::isAnimated()const {
	return clips;
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

