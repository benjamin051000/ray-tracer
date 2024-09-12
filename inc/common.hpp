#pragma once

#include <cmath>
#include <limits>
#include <memory>

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

#if defined(__FAST_MATH__)
// Infinity with -ffast-math has undefined behavior apparently.
constexpr auto infinity = std::numeric_limits<double>::max();
#else
constexpr auto infinity = std::numeric_limits<double>::infinity();
#endif
constexpr auto PI = 3.1415926535897932385;

// Common functions
[[nodiscard]] constexpr double deg_to_rad(const double deg) {
	return deg * PI / 180;
}

// namespace random {
[[nodiscard]] double random_double();

[[nodiscard]] double random_double(const double min, const double max);
[[nodiscard]] int random_int(const int min, const int max);

// } // end of namespace random
