#include "common.hpp"

// Common functions
[[nodiscard]] double random_double() {
	return rand() / (RAND_MAX + 1.0);
}

[[nodiscard]] double random_double(double min, double max) {
	return min + (max - min) * random_double();
}

[[nodiscard]] int random_int(int min, int max) {
	return static_cast<int>(random_double(min, max + 1));
}
