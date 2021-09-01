#include "pch.h"
#include "Random.h"
#include <random>

std::random_device globalSeed;
std::mt19937 globalRandomEngine(globalSeed());

float Random::RandomFloat(const float aMin, const float aMax)
{
	std::uniform_real_distribution<float> dist(aMin, aMax);
	return dist(globalRandomEngine);
}

int Random::RandomInt(const int aMin, const int aMax)
{
	std::uniform_int_distribution<int> dist(aMin, aMax);
	return dist(globalRandomEngine);
}
