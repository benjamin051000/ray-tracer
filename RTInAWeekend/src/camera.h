#pragma once
#include "common.h"

class camera {
public:
	/*vfov is top to bottom in degrees. vup determines roll of camera.*/
	camera(
		point3 lookfrom, 
		point3 lookat,
		vec3 vup,
		float vfov, // Vertical FOV in degrees
		float aspect,
		float aperture,
		float focus_dist,
		double t0 = 0,
		double t1 = 1) 
	{
		float theta = deg_to_rad(vfov),
			half_height = tan(theta / 2),
			viewport_height = 2 * half_height,
			viewport_width = aspect * viewport_height;

		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		horizontal = viewport_width * focus_dist * u;
		vertical = viewport_height * focus_dist * v;
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;
		
		
		/*The aperture and focus_dist determine defocus blur (depth of field).
		Setting aperture to 0 will 'disable' this effect.*/
		lens_radius = aperture / 2;
		time0 = t0;
		time1 = t1;
	}

	ray get_ray(float s, float t) {
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() + v * rd.y();
		return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset, random_float(time0, time1));
	}

private:
	float lens_radius;
	vec3 u, v, w;
	point3 origin, lower_left_corner;
	vec3 horizontal, vertical;
	float time0, time1;
};