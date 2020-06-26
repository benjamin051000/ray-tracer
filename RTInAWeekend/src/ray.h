#pragma once
#include "vec3.h"

struct ray {
	ray() {}

	ray(const point3& origin, const vec3& direction, double time = 0) : 
		orig(origin), dir(direction), tm(time) {}

	point3 origin() const { return orig; }
	vec3 direction() const { return dir; }
	point3 at(const float t) const { return orig + t * dir; }
	double time() const { return tm; }


	//Points (vectors) that make up the ray.
	point3 orig;
	vec3 dir;
	double tm = 0;
};
