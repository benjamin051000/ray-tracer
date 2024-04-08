#pragma once
#include "vec3.hpp"

struct ray {
	ray();

	ray(const point3& origin, const vec3& direction, double time = 0);

	[[nodiscard]] point3 at(const double t) const noexcept;

	//Points (vectors) that make up the ray.
	point3 origin;
	vec3 direction;
	double time;
};

