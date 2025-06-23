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
	std::chrono::duration<float> Timer::getDuration()const
	{
		return duration;
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
	bool FrameTimer::isSpareTime()
	{
		return getDuration() < maxFrameDuration;
	}
	std::chrono::duration<float> FrameTimer::getDelay_default()const
	{
		std::chrono::duration<float> diff = maxFrameDuration - getDuration();
		return (diff.count() < 0.f) ? std::chrono::duration<float>(0) : diff;
	}
	std::chrono::milliseconds FrameTimer::getDelay_milliseconds()const
	{
		std::chrono::duration<float> diff = maxFrameDuration - getDuration();
		return (diff.count() < 0.f) ?
			std::chrono::milliseconds(0) :
			std::chrono::duration_cast<std::chrono::milliseconds>(diff);
	}
	std::chrono::microseconds FrameTimer::getDelay_microseconds()const
	{
		std::chrono::duration<float>diff = maxFrameDuration - getDuration();
		return(diff.count() < 0.f) ?
			std::chrono::microseconds(0) :
			std::chrono::duration_cast<std::chrono::microseconds>(diff);
	}

}