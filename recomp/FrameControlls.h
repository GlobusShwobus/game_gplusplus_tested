#pragma once

#include "SDL3/SDL.h"

namespace badEngine {

	//frame counter controlls
	namespace FCC {

		class FrameControlls {

			static constexpr uint32_t millisecondsPerSecond = 1000;
			static constexpr uint32_t defaultFPS = 60;

			const uint32_t maxFrameDuration = millisecondsPerSecond / defaultFPS;
			uint32_t fps = 0;

			uint64_t frameStart = 0;
			uint64_t frameEnd = 0;
			uint64_t duration = 0;

		public:

			FrameControlls(const uint32_t FPSGoal) :fps(FPSGoal), maxFrameDuration(millisecondsPerSecond / fps) {}

			void start()
			{
				frameStart = SDL_GetTicks();
			}
			void end()
			{
				frameEnd = SDL_GetTicks();
				duration = frameEnd - frameStart;
			}
			bool isSpareTime()const
			{
				return duration < maxFrameDuration;
			}
			int64_t getDelayDuration()const//will return a negative value if isSpareTime not taken into account
			{
				return maxFrameDuration - duration;
			}

		};

	}


}