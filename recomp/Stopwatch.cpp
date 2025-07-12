#include "Stopwatch.h"
namespace badEngine {

	void Stopwatch::start()
	{
		startPoint = std::chrono::steady_clock::now();
	}
	void Stopwatch::end()
	{
		endPoint = std::chrono::steady_clock::now();
		duration = endPoint - startPoint;
	}
	std::chrono::duration<float> Stopwatch::getDuration_float()const
	{
		return duration;
	}
	std::chrono::milliseconds Stopwatch::getDuration_millisecond()const
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(duration);
	}
	std::chrono::microseconds Stopwatch::getDuration_microseconds()const
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
		limit = std::chrono::duration<float>(ONE_SECOND / fps);
	}
	bool FrameTimer::isSpareTime()const
	{
		return getDuration_float() < limit;
	}
	const std::chrono::duration<float> FrameTimer::getLimit_float()const {
		return limit;
	}
	const std::chrono::milliseconds FrameTimer::getLimit_milliseconds()const {
		return std::chrono::duration_cast<std::chrono::milliseconds>(limit);
	}
	const std::chrono::microseconds FrameTimer::getLimit_microseconds()const {
		return std::chrono::duration_cast<std::chrono::microseconds>(limit);
	}

}