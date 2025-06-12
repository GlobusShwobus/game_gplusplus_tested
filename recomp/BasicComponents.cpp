#include "BasicComponents.h"
namespace badEngine {
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

	int RandomNumberGenerator::getRand(int min, int max) {
		return std::uniform_int_distribution<int>(min, max)(*rng);
	}
}