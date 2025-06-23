#include "Timer.h"

namespace badEngine {

	void Timer::start()
	{
		startPoint = std::chrono::steady_clock::now();
	}
	void Timer::end()
	{
		endPoint = std::chrono::steady_clock::now();
		duration = endPoint - startPoint;
	}
	std::chrono::duration<float> Timer::getDuration_float()const
	{
		return duration;
	}
	std::chrono::milliseconds Timer::getDuration_millisecond()const
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(duration);
	}
	std::chrono::microseconds Timer::getDuration_microseconds()const
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(duration);
	}

	uint32_t FrameTimer::getFPS()const
	{
		return fps;
	}
	void FrameTimer::setFPS(uint32_t FPS)
	{
		fps = FPS;
		maxFrameDuration = std::chrono::duration<float>(ONE_SECOND / fps);
	}
	bool FrameTimer::isSpareTime()const
	{
		return getDuration_float() < maxFrameDuration;
	}
	const std::chrono::duration<float> FrameTimer::getMaxFrameDuration_float()const {
		return maxFrameDuration;
	}
	const std::chrono::milliseconds FrameTimer::getMaxFrameDuration_milliseconds()const {
		return std::chrono::duration_cast<std::chrono::milliseconds>(maxFrameDuration);
	}
	const std::chrono::microseconds FrameTimer::getMaxFrameDuration_microseconds()const {
		return std::chrono::duration_cast<std::chrono::microseconds>(maxFrameDuration);
	}

}