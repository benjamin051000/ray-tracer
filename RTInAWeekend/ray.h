#pragma once
#include "vec3.h"

struct ray {
	ray() {}
	ray(const vec3 &a, const vec3 &b) { A = a, B = b; }
	vec3 origin() const { return A; }
	vec3 direction() const { return B; }
	vec3 point_at_parameter(const float t) const { return A + t * B; }

	//Points (vectors) that make up the ray. Points from A to B.
	vec3 A, B;
};