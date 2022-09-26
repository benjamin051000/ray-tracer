#pragma once
#include "common.h"

class camera {
public:
	/* Default Constructor */
	camera() : camera(point3(0, 0, -1), point3(0, 0, 0), vec3(0, 1, 0), 40, 1, 0, 10) {}

	/*vfov is top to bottom in degrees. vup determines roll of camera.*/
	camera(
		point3 lookfrom,
		point3 lookat,
		vec3 vup,
		double vfov, // Vertical FOV in degrees
		double aspect_ratio,
		double aperture,
		double focus_dist,
		double _time0 = 0,
		double _time1 = 0
	) {
		double theta = deg_to_rad(vfov),
			half_height = tan(theta / 2),
			viewport_height = 2 * half_height,
			viewport_width = aspect_ratio * viewport_height;

		
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		
		origin = lookfrom;
		horizontal = viewport_width * focus_dist * u;
		vertical = viewport_height * focus_dist * v;
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;


		/*The aperture and focus_dist determine defocus blur (depth of field).
		Setting aperture to 0 will 'disable' this effect.*/
		lens_radius = aperture / 2;
		time0 = _time0;
		time1 = _time1;
	}

	ray get_ray(double s, double t) {
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() + v * rd.y();
		return ray(
			origin + offset, 
			lower_left_corner + s * horizontal + t * vertical - origin - offset,
			random_double(time0, time1)
		);
	}

private:
	double lens_radius;
	vec3 u, v, w;
	point3 origin, lower_left_corner;
	vec3 horizontal, vertical;
	double time0, time1;
};