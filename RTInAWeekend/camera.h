#pragma once
#include "ray.h"

struct camera {
	camera() {
		lower_left_corner = vec3(-2, -1, -1);
		horiz = vec3(4, 0, 0);
		vert = vec3(0, 2, 0);
		origin = vec3(0, 0, 0);
	}

	ray get_ray(float u, float v) { return ray(origin, lower_left_corner + u * horiz + v * vert - origin); }

	vec3 origin, lower_left_corner, horiz, vert;
};