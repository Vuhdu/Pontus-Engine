#pragma once
#include <assert.h>
#include <random>

#define CU_PI (3.14159265358979323846f)

namespace CommonUtilities
{
	template<typename T>
	T Min(const T& aFirst, const T& aSecond)
	{
		return (aFirst < aSecond) ? aFirst : aSecond;
	}

	template<typename T>
	T Max(const T& aFirst, const T& aSecond)
	{
		return (aFirst > aSecond) ? aFirst : aSecond;
	}

	template<typename T>
	T Clamp(const T& aClampValue, const T& aMin, const T& aMax)
	{
		assert(aMax >= aMin && "aMin can't be less than aMax");
		return (aClampValue <= aMin) ? aMin : (aClampValue >= aMax) ? aMax : aClampValue;
	}

	template<typename T>
	T Abs(const T& aValue)
	{
		return (aValue < 0) ? -aValue : aValue;
	}

	template<typename T>
	void Swap(T& aFirst, T& aSecond)
	{
		T temp = aFirst;
		aFirst = aSecond;
		aSecond = temp;
	}

	template<typename T>
	T Lerp(const T& aFirst, const T& aSecond, const float aTime)
	{
		return (aFirst + aTime * (aSecond - aFirst));
		//return (1 - aTime) * aFirst + aTime * aSecond;
	}
}
namespace CU = CommonUtilities;