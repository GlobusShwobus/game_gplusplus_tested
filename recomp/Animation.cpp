#include "Animation.h"

namespace badEngine {
	Animation::Animation(int x, int y, int w, int h, int count, float holdTime)
		:holdTime(holdTime), frameWidth(w), frameHeight(h)
	{
		for (int i = 0; i < count; i++) {
			frames.emplace_back(Vek2((float)x + (w * i), (float)y));
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
}
