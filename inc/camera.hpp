#pragma once
#include "common.hpp"
#include "ray.hpp"

class camera {
	double lens_radius;
	vec3 u, v, w;
	const point3 origin;
	point3 lower_left_corner;
	vec3 horizontal, vertical;
	const double time0, time1;

public:
	camera() : camera(point3{0, 0, -1}, point3{0, 0, 0}, vec3{0, 1, 0}, 40, 1, 0, 10) {}

	camera(
		// These points define a math-vector (->) that is the direction the camera faces.
		const point3 lookfrom, 
		const point3 lookat, 

		const vec3 vup, // Determines roll axis of camera
		const double vfov, // Vertical FOV in degrees
		const double aspect_ratio,
		const double aperture,
		const double focus_dist,
		const double time0 = 0,
		const double time1 = 0
	)
	: origin(lookfrom), time0(time0), time1(time1)
	{
		const double theta = deg_to_rad(vfov),
			half_height = tan(theta / 2),
			viewport_height = 2 * half_height,
			viewport_width = aspect_ratio * viewport_height;

		
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		
		horizontal = viewport_width * focus_dist * u;
		vertical = viewport_height * focus_dist * v;
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

		/*The aperture and focus_dist determine defocus blur (depth of field).
		Setting aperture to 0 will 'disable' this effect.*/
		lens_radius = aperture / 2;
	}

	ray get_ray(double s, double t) const {
		const vec3 rd = lens_radius * random_in_unit_disk();
		const vec3 offset = u * rd.x() + v * rd.y();

		return ray(
			origin + offset, 
			lower_left_corner + s * horizontal + t * vertical - origin - offset,
			random_double(time0, time1)
		);
	}
};

