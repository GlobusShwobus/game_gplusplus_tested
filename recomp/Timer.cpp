#include "Timer.h"

namespace badEngine {

	void Timer::start()
	{
		startPoint = std::chrono::steady_clock::now();
	}
	void Timer::end()
	{
		endPoint = std::chrono::steady_clock::now();
		duration = std::chrono::duration_cast<std::chrono::milliseconds>(endPoint - startPoint);
	}
	std::chrono::milliseconds Timer::getDuration()const
	{
		return duration;
	}

	bool FrameTimer::isSpareTime()
	{
		return getDuration() < maxFrameDuration;
	}
	std::chrono::milliseconds FrameTimer::getDelayDuration()
	{
		std::chrono::milliseconds diff = maxFrameDuration - getDuration();
		return diff.count() < 0 ? std::chrono::milliseconds(0) : diff;
	}

}