#include "Animation.h"
#include <assert.h>

namespace badEngine {
	Animation::Animation(const Vek2& pos, const Vek2& imageSize, int frameWidth, int frameHeight, int count, float holdTime)
		:holdTime(holdTime), frameWidth(frameWidth), frameHeight(frameHeight)
	{
		assert((imageSize.x / frameWidth) <= count);
		assert(imageSize.y >= frameHeight);

		for (int i = 0; i < count; i++) {
			frames.emplace_back(Vek2((float)pos.x + (frameWidth * i), (float)pos.y));
		}
	}
	void Animation::advance() {
		++curFrame;
		if (curFrame >= frames.size()) {
			curFrame = 0;
		}
	}
	void Animation::update(float dt) {
		curFrameTime += dt;
		while (curFrameTime >= holdTime) {
			advance();
			curFrameTime = holdTime;
		}
	}
	Rectangle Animation::getFrame() {
		return Rectangle(frames[curFrame].x, frames[curFrame].y, frameWidth, frameHeight);
	}
}
