#pragma once
#include "ray.h"
#include <math.h>
constexpr double PI = 3.14159265359; //TODO maybe refactor this?


float random() {
	//From stackoverflow
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

vec3 random_in_unit_disk() {
	vec3 p;
	do {
		p = 2 * vec3(random(), random(), 0) - vec3(1, 1, 0);
	} while (dot(p, p) >= 1);
	return p;
}



class camera {
public:
	/*vfov is top to bottom in degrees. vup determines roll of camera.*/
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist) {
		/*The aperture and focus_dist determine defocus blur (depth of field).
		Setting aperture to 0 will 'disable' this effect.*/
		lens_radius = aperture / 2;

		float theta = vfov * PI / 180,
			half_height = tan(theta / 2),
			half_width = aspect * half_height;

		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		lower_left_corner = origin - half_width * focus_dist * u 
			- half_height * focus_dist * v 
			- focus_dist * w;
		horiz = 2 * half_width * focus_dist * u;
		vert = 2 * half_height * focus_dist * v;
	}

	ray get_ray(float s, float t) {
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() + v * rd.y();
		return ray(origin + offset, lower_left_corner + s * horiz + t * vert - origin - offset); 
	}
	
	float lens_radius;
	vec3 u, v, w;
	vec3 origin, lower_left_corner, horiz, vert;
};