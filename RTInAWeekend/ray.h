#pragma once
#include "vec3.h"

struct ray {
	ray() {}
	ray(const vec3 &a, const vec3 &b) { A = a, B = b; }
	vec3 origin() const { return A; } //Could the return type be const?
	vec3 direction() const { return B; }
	vec3 point_at_parameter(const float t) const { return A + t * B; }

	vec3 A, B;
};