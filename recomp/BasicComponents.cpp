#include "BasicComponents.h"
namespace badEngine {

	int RandomNumberGenerator::getRand(int min, int max) {
		return std::uniform_int_distribution<int>(min, max)(*rng);
	}
}