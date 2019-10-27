#include <iostream>
#include "ray.h"
#include <fstream>

float hit_sphere(const vec3 &center, const float radius, const ray &r) {
	vec3 oc = r.origin() - center;
	
	//Calculate discriminant
	float a = dot(r.direction(), r.direction());
	float b = 2 * dot(oc, r.direction());
	float c = dot(oc, oc) - radius*radius;
	float discriminant = b*b - 4*a*c;

	if (discriminant < 0) {
		return -1.0;
	}
	else {
		//Send nearest hit point
		return (-b - sqrt(discriminant)) / (2.f * a);
	}
}

vec3 color(const ray &r) {
	float t = hit_sphere(vec3(0, 0, -1), 0.5, r);
	
	if (t > 0.0) {
		vec3 N = unit_vector(r.point_at_parameter(t) - vec3(0, 0, -1));
		return 0.5 * vec3(N.x() + 1, N.y() + 1, N.z() + 1);
	}

	vec3 unit_direction = r.direction();
	unit_direction.make_unit_vector();
	t = 0.5 * (unit_direction.y() + 1.0);

	return (1.0 - t) * vec3(1, 1, 1) + t * vec3(.5, .7, 1);
}

int main() {
	int nx = 200;
	int ny = 100;
	
	std::ofstream out("image.ppm");
	
	out << "P3\n" << nx << " " << ny << "\n255\n";
	
	const vec3 lower_left_corner(-2, -1, -1);
	const vec3 horizontal(4, 0, 0);
	const vec3 vertical(0, 2, 0);
	const vec3 origin(0, 0, 0);
	
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);

			ray r(origin, lower_left_corner + u * horizontal + v * vertical);
			vec3 col = color(r);
			int ir = int(255.99 * col[0]),
				ig = int(255.99 * col[1]),
				ib = int(255.99 * col[2]);
			out << ir << " " << ig << " " << ib << "\n";
		}
	}
	out.close();
}