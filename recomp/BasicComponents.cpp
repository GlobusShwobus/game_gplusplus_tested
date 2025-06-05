#include "BasicComponents.h"

AnimController::AnimController(const std::vector<FrameMap>& reels) :clips(reels) {
	if (!reels.empty()) {
		current = &reels.front();
	}
}
void AnimController::moveFrame() {
	frameTimer++;
	if (frameTimer < frameDelay)return;
	frameTimer = 0;
	frameIndex++;

	if (current->isLooping) {
		if (frameIndex >= current->frames.size())
			frameIndex = 0;
	}
	else {
		if (frameIndex >= current->frames.size())
			frameIndex = current->frames.size() - 1;
	}
}
bool AnimController::setIfNew(const AnimID id) {
	if (current->id == id) return false;

	for (auto& each : clips) {
		if (each.id == id) {
			current = &each;
			frameIndex = 0;
			return true;
		}
	}
	return false;
}
void AnimController::applySourceFromFrame(SDL_FRect& rect)const {
	const SDL_Rect& cr = current->frames[frameIndex];
	rect = { (float)cr.x, (float)cr.y,(float)cr.w,(float)cr.h };
}
void RectRayProjection::projectRay(const SDL_FRect& origin, const SDL_FPoint& vel, const SDL_FRect& target) {
	SDL_FPoint inverse = { 1.0f / vel.x, 1.0f / vel.y };
	entry = { (target.x - origin.x) * inverse.x,(target.y - origin.y) * inverse.y };
	exit = { (target.x + target.w - origin.x) * inverse.x, (target.y + target.h - origin.y) * inverse.y };
}
bool EntityEvent::containsEvent(MyPersonalEvents event)const {
	return events & event;
}
bool EntityEvent::containsEvent(const Uint64& events, MyPersonalEvents event) {
	return events & event;
}
void EntityEvent::setEvent(const int events) {
	this->events |= events;
}
void EntityEvent::flushEvents() {
	events = 0;
}