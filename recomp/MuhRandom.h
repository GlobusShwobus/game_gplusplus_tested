#pragma once

#include <random>

namespace badEngine {

	namespace RNG {

		class RandomNumberGenerator {
			std::random_device rd;
			std::unique_ptr<std::mt19937> rng;

		public:
			RandomNumberGenerator() : rng(std::make_unique<std::mt19937>(rd())) {}

			int getRand(int min, int max);
		};

	}

}