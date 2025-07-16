#pragma once

#include <vector>
#include "Vek2.h"

namespace badEngine {

	class Animation {
		std::vector<Vek2> frames;
		int frameWidth = 0;
		int frameHeight = 0;
		int curFrame = 0;
		float holdTime;
		float curFrameTime = 0.0f;
	private:
		void advance();
	public:
		Animation(int x, int y, int w, int h, int count, float holdTime);
		void update(float dt);
	};
}