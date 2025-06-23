#pragma once

#include <chrono>

namespace badEngine {

	class Timer {
		std::chrono::time_point<std::chrono::steady_clock> startPoint;
		std::chrono::time_point<std::chrono::steady_clock> endPoint;

		std::chrono::milliseconds duration;
	public:
		Timer() = default;

		void start();
		void end();
		std::chrono::milliseconds getDuration()const;
	};

	class FrameTimer: public Timer {
		static constexpr uint32_t millisecondsPerSecond = 1000;
		static constexpr uint32_t defaultFPS = 60;
		
		std::chrono::milliseconds maxFrameDuration;

		uint32_t fps = 0;

	public:
		FrameTimer(uint32_t fps = defaultFPS) :fps(fps), maxFrameDuration(std::chrono::milliseconds(millisecondsPerSecond / fps)) {}

		bool isSpareTime();
		std::chrono::milliseconds getDelayDuration();

	};
}