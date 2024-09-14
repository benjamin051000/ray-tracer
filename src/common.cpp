#include "common.hpp"
#include <random>
#include "pcg_random.hpp"

[[nodiscard]] double random_double() {
	// This is expensive
	static thread_local pcg32 generator{pcg_extras::seed_seq_from<std::random_device>()};
	// This is cheap
	std::uniform_real_distribution uniform_dist;
	return uniform_dist(generator);
}

[[nodiscard]] double random_double(const double min, const double max) {
	return min + (max - min) * random_double();
}

[[nodiscard]] int random_int(const int min, const int max) {
	return static_cast<int>(random_double(min, max + 1));
}
