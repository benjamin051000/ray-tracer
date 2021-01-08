#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const float infinity = std::numeric_limits<float>::infinity();
const double PI = 3.1415926535897932385;


// Common functions
inline double deg_to_rad(double deg) {
	return deg * PI / 180;
}

inline double clamp(double x, double min, double max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

inline float random_float() {  // TODO deprecate in favor of random_double().
	// Returns a random real in [0,1).
	return rand() / (RAND_MAX + 1.f);
}

inline float random_float(float min, float max) {
	return min + (max - min) * random_float();
}

inline double random_double() {
	return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
	return min + (max - min) * random_double();
}

inline int random_int(int min, int max) {
	return static_cast<int>(random_float(min, max + 1));
}



// Common headers
#include "ray.h"
//#include "vec3.h" // Included in ray.h

vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		p = 2.0 * vec3(random_float(), random_float(), random_float()) - vec3(1, 1, 1);
	} while (p.length_squared() >= 1.0);
	return p;
}