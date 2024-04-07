#pragma once
#include "vec3.hpp"

struct ray {
	ray();

	ray(const point3& origin, const vec3& direction, double time = 0);

	point3 origin() const;
	vec3 direction() const;
	point3 at(const double t) const;
	double time() const;

	//Points (vectors) that make up the ray.
	point3 orig;
	vec3 dir;
	double tm = 0;
};

