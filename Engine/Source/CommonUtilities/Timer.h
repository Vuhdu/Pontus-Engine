#pragma once
#include <chrono>
#include <vector>
#include <functional>

namespace CommonUtilities
{
	struct StopTimer
	{
		float myTimer = 0.0f;
		float myStartTime = 0.0f;
		std::function<void(void)> myCallback;
		bool myIsRepeatable = false;
	};

	class Timer
	{
	public:
		Timer();
		Timer(const Timer& aTimer) = delete;
		Timer& operator=(const Timer& aTimer) = delete;

		void Update();
		float GetDeltaTime() const;
		float GetUnscaledDeltaTime() const;
		double GetTotalTime() const;

		void SetTimeScale(const float aScaleValue);
		void ResetDeltaTime();

		void SetTimer(const float aTime, const std::function<void(void)>& aCallbackFunction, const bool isRepeatable = false);
		void ClearAllTimers();

	private:
		void UpdateStopTimers();

	private:
		std::chrono::high_resolution_clock myTimer;
		std::chrono::time_point<std::chrono::high_resolution_clock> myUpdateStart;
		float myDeltaTime = 0.0f;
		float myTimeScale = 0.0f;
		double myTotalTime = 0.0f;

		std::vector<StopTimer> myStopTimers;
	};
}
namespace CU = CommonUtilities;