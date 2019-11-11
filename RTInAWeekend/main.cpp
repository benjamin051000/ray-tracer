#include <iostream>
#include <fstream>
#include "hittable_list.h"
#include "sphere.h"
#include "float.h"
#include "camera.h"
#include "material.h"

//Calculates the color of the pixel.
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
		//What is this block for? If the ray doesn't hit anything in the world, isn't the color just white?
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1);
		return (1 - t) * vec3(1, 1, 1) + t * vec3(0.5, 0.7, 1);
	}
}

hittable* random_scene();

int main() {
	//Resolution
	const int nx = 800, ny = 600;
	//Samples per pixel
	const unsigned int spp = 10;
	
	//Set up the output file
	std::ofstream out("image.ppm");
	out << "P3\n" << nx << " " << ny << "\n255\n";
	
	//Initialize the objects in the scene
	/*const unsigned int list_size = 5;
	hittable *list[list_size];
	list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
	list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));*/
	
	

	/*hittable *world = new hittable_list(list, list_size);*/
	hittable* world = random_scene();
	
	//Set up camera
	vec3 lookfrom(13, 2, 3), lookat(0, 0, 0);
	float dist_to_focus = (lookfrom - lookat).length(), //can be any number ig
		aperture = 0;
	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus);


	/*----------Render the image----------*/
	double percentage = 0;
	const double percentChange = 100.0 / ny ;

	//Iterate through each pixel. Y iterates backwards.
	for (int y = ny - 1; y >= 0; y--) {
		for (unsigned int x = 0; x < nx; x++) {
			vec3 col(0, 0, 0);
			//Repeat samples-per-pixel times
			for (unsigned int s = 0; s < spp; s++) {

				//Get a random ray in the pixel
				float u = float(x + random()) / float(nx);
				float  v = float(y + random()) / float(ny);

				ray r = cam.get_ray(u, v);
				col += color(r, world, 0);
			}
			
			//Average the color value
			col /= float(spp);
			//Not sure why but square root the colors
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			
			//Write the rgb values
			const int  ir = int(255.99 * col[0]);
			const int  ig = int(255.99 * col[1]);
			const int  ib = int(255.99 * col[2]);
			out << ir << " " << ig << " " << ib << "\n";
		}
		percentage += percentChange;
		std::cout << "Percent complete: " << percentage << "%" << std::endl;
	}
	out.close();
}


#define random_double() random()

hittable* random_scene() {
	int n = 500;
	hittable** list = new hittable * [n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = random_double();
			vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.8) {  // diffuse
					list[i++] = new sphere(center, 0.2,
						new lambertian(vec3(random_double() * random_double(),
							random_double() * random_double(),
							random_double() * random_double())
						)
					);
				}
				else if (choose_mat < 0.95) { // metal
					list[i++] = new sphere(center, 0.2,
						new metal(vec3(0.5 * (1 + random_double()),
							0.5 * (1 + random_double()),
							0.5 * (1 + random_double())),
							0.5 * random_double()));
				}
				else {  // glass
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new hittable_list(list, i);
}