#include <iostream>
#include <fstream>
#include "hittable_list.h"
#include "sphere.h"
#include "float.h"
#include "camera.h"
#include "material.h"

float random();

vec3 color(const ray &r, hittable *world, int depth) {
	hit_record rec;

	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.material->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1);
		return (1 - t) * vec3(1, 1, 1) + t * vec3(0.5, 0.7, 1);
	}
}



int main() {
	//Resolution
	unsigned int nx = 800, ny = 400;
	//Samples per pixel
	int spp = 100;
	
	std::ofstream out("image.ppm");
	
	out << "P3\n" << nx << " " << ny << "\n255\n";
	
	const unsigned int list_size = 4;
	hittable *list[list_size];
	list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, new metal(vec3(0.8, 0.8, 0.8), 1));

	

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
				col += color(r, world, 0);
			}

			col /= float(spp);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			
			const int  ir = int(255.99 * col[0]);
			const int  ig = int(255.99 * col[1]);
			const int  ib = int(255.99 * col[2]);
			out << ir << " " << ig << " " << ib << "\n";
		}
		std::cout << "Percent complete: " << 100 - float(y) / ny * 100 << "%\n";
	}
	out.close();
}