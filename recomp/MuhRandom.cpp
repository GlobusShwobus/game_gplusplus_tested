#include "MuhRandom.h"

namespace badEngine {

	namespace RNG {

		int RandomNumberGenerator::getRand(int min, int max) {
			return std::uniform_int_distribution<int>(min, max)(*rng);
		}

	}

}