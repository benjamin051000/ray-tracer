#pragma once

#include <cmath>
#include <limits>
#include <memory>

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

constexpr auto infinity = std::numeric_limits<float>::infinity();
constexpr auto PI = 3.1415926535897932385;

// Common functions
[[nodiscard]] constexpr double deg_to_rad(const double deg) {
	return deg * PI / 180;
}

[[nodiscard]] constexpr double clamp(const double x, const double min, const double max) {
	if (x < min)
		return min;
	if (x > max)
		return max;
	return x;
}

[[nodiscard]] double random_double();

[[nodiscard]] double random_double(const double min,
                                             const double max);

[[nodiscard]] int random_int(const int min, const int max);
