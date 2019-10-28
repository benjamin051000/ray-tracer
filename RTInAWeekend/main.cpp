#include <iostream>
#include <fstream>
#include "hittable_list.h"
#include "sphere.h"
#include "float.h"
#include "camera.h"

vec3 color(const ray &r, hittable *world) {
	hit_record rec;
	if (world->hit(r, 0, FLT_MAX, rec)) {
		return 0.5 * vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
	}
	else {
		vec3 unit_dir = unit_vector(r.direction());
		float t = 0.5 * (unit_dir.y() + 1);
		return (1.0 - t) * vec3(1, 1, 1) + t * vec3(0.5, 0.7, 1);
	}
}

//From stackoverflow
float random() {
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

int main() {
	//Resolution
	unsigned int nx = 400, ny = 200;
	//Samples per pixel
	int spp = 10;
	
	std::ofstream out("image.ppm");
	
	out << "P3\n" << nx << " " << ny << "\n255\n";
	
	const unsigned int list_size = 2;
	hittable *list[list_size];
	list[0] = new sphere(vec3(0, 0, -1), 0.5);
	list[1] = new sphere(vec3(0, -100.5, -1), 100);
	

	hittable *world = new hittable_list(list, list_size);
	camera cam;

	//Iterate through each pixel
	for (int y = ny - 1; y >= 0; y--) {
		for (unsigned int x = 0; x < nx; x++) {
			vec3 col(0, 0, 0);

			for (int s = 0; s < spp; s++) {
				float u = float(x + random()) / float(nx);
				float  v = float(y + random()) / float(ny);

				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.0);
				col += color(r, world);
			}

			col /= float(spp);
			int  ir = int(255.99 * col[0]);
			int  ig = int(255.99 * col[1]);
			int  ib = int(255.99 * col[2]);
			out << ir << " " << ig << " " << ib << "\n";
		}
		std::cout << "Percent complete: " << 100 - float(y) / ny * 100 << "%\n";
	}
	out.close();
}