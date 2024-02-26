#include "MyMath.h"
#include<random>

using namespace DirectX;

float MyMath::RandomMTFloat(float min, float max)
{
	std::random_device seed;
	std::mt19937_64 engine(seed());
	std::uniform_real_distribution<float> dist(min, max);

	return dist(engine);
}

int MyMath::RandomMTInt(int min, int max)
{
	std::random_device seed;
	std::mt19937_64 engine(seed());
	std::uniform_real_distribution<> dist(min, max);

	return (int)dist(engine);
}