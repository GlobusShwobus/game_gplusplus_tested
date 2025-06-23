#include "MuhRandom.h"

namespace badEngine {

	namespace RNG {

		int RandomNumberGenerator::getRandInt(int min, int max)
		{
			return std::uniform_int_distribution<int>(min, max)(*rng);
		}

		float RandomNumberGenerator::getRandFloat(float min, float max)
		{
			return std::uniform_real_distribution<float>(min, max)(*rng);
		}

	}

}