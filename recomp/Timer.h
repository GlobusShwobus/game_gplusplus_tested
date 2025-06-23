#pragma once

#include <chrono>

namespace badEngine {

	class Timer {
		std::chrono::time_point<std::chrono::steady_clock> startPoint;
		std::chrono::time_point<std::chrono::steady_clock> endPoint;

		std::chrono::duration<float> duration;
	public:
		Timer() = default;

		void start();
		void end();
		std::chrono::duration<float> getDuration_float()const;
		std::chrono::milliseconds getDuration_millisecond()const;
		std::chrono::microseconds getDuration_microseconds()const;
	};

	class FrameTimer: public Timer {
		static constexpr float ONE_SECOND = 1.0f;
		static constexpr uint32_t defaultFPS = 60;
		
		std::chrono::duration<float> maxFrameDuration;
		uint32_t fps = 0;

	public:
		FrameTimer(uint32_t fps = defaultFPS) :fps(fps), maxFrameDuration(ONE_SECOND /fps) {}

		uint32_t getFPS()const;
		void setFPS(uint32_t FPS);

		bool isSpareTime()const;
		const std::chrono::duration<float> getMaxFrameDuration_float()const;
		const std::chrono::milliseconds getMaxFrameDuration_milliseconds()const;
		const std::chrono::microseconds getMaxFrameDuration_microseconds()const;
	};
}