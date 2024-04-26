#pragma once
#include "vec3.hpp"

struct ray {
	constexpr ray() = default;

	constexpr ray(const point3& origin, const vec3& direction, const double time) :
		origin(origin), direction(direction), time(time) {}

	[[nodiscard]] constexpr point3 at(const double t) const { return origin + t * direction; }

	//Points (vectors) that make up the ray.
	point3 origin;
	vec3 direction;
	double time = 0;
};

