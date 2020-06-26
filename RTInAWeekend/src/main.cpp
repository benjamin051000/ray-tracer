#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#pragma warning(push, 3) // Removes warnings
#include "stb_image_write.h"
#pragma warning(pop)

#include "common.h"

#include "hittable_list.h"
#include "camera.h"
#include "material.h"

#include "sphere.h"
#include "moving_sphere.h"

using std::chrono::steady_clock;
using std::chrono::duration_cast;

//Calculates the color of the pixel.
vec3 ray_color(const ray &r, hittable &world, int depth) {
	hit_record rec;
	
	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0) return color(0.f, 0.f, 0.f);

	if (world.hit(r, 0.001, infinity, rec)) {
		ray scattered;
		color attenuation;

		if (rec.material->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);
		
		return vec3(0, 0, 0);
	}
	
	//What is this block for? If the ray doesn't hit anything in the world, isn't the color just white?
	vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5f * (unit_direction.y() + 1.f);
	return (1 - t) * color(1, 1, 1) + t * color(0.5f, 0.7f, 1);
}


hittable_list random_scene();
//std::string task(const int x, const int y, hittable_list world, const int max_depth, camera cam, const unsigned int spp, int nx, int ny);


int main() {
	//Resolution
	const unsigned int nx = 640, ny = 480;
	//Samples per pixel
	const unsigned int spp = 20;
	// Maximum recursive depth for ray reflections.
	const int max_depth = 50;
	
	//Initialize the objects in the scene
	hittable_list world = random_scene();
	
	//Set up camera
	vec3 lookfrom(13, 2, 3), lookat(0, 0, 0);
	float dist_to_focus = (lookfrom - lookat).length(),
		  aperture = 0;
	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus);

	// Array to hold pixel colors (x * y * num_pixels)
	unsigned char* pixels = new unsigned char[nx * ny * 3];
	unsigned int pixel_idx = 0;

	/*----------Render the image----------*/
	double percentage = 0;
	const double percentChange = 100.0 / ny ;

	auto start = steady_clock::now();

	// Main loop
	for (int y = ny-1; y >= 0; y--) {

		auto row_start = steady_clock::now();

		for (unsigned int x = 0; x < nx; x++) {
			color col(0, 0, 0);
			//Repeat samples-per-pixel times
			for (unsigned int s = 0; s < spp; s++) {

				//Get a random ray in the pixel
				float u = float(x + random_float()) / float(nx);
				float  v = float(y + random_float()) / float(ny);

				ray r = cam.get_ray(u, v);
				col += ray_color(r, world, max_depth);
			}

			//Average the color value
			col /= float(spp);
			//Not sure why but square root the colors
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

			//Write the rgb values
			unsigned char ir = int(255 * col[0]), 
						  ig = int(255 * col[1]), 
						  ib = int(255 * col[2]);

			pixels[pixel_idx++] = ir;
			pixels[pixel_idx++] = ig;
			pixels[pixel_idx++] = ib;
		}

		auto row_done = steady_clock::now();
		percentage += percentChange;
		auto elapsed_row = duration_cast<std::chrono::seconds>(row_done - row_start).count();
		
		std::cout << percentage << "% complete (Row time: " << elapsed_row << " s)\n";

		/*auto est_time_remaining = (elapsed_row / percentage) * (100 - percentage);
		std::cout << "\tEst. time remaining: " << est_time_remaining << " s" << std::endl;*/
	}
	

	auto stop = steady_clock::now();

	auto total_seconds = duration_cast<std::chrono::minutes>(stop - start).count();
	std::cout << "Total render time: " << total_seconds << " min" << std::endl;

	//Set up the output file
	const std::string filename = "render_" + std::to_string(nx) + "_" + std::to_string(ny) + "_" + std::to_string(spp) + ".jpg";
	stbi_write_jpg(filename.c_str(), nx, ny, 3, pixels, 100);

}


hittable_list random_scene() {
	hittable_list world;

	world.add(make_shared<sphere>(
		vec3(0, -1000, 0), 
		1000, 
		make_shared<lambertian>(color(.5f, .5f, .5f))
	));

	// Creates 484 spheres of varying materials and positions.
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {

			float choose_mat = random_float();
			point3 center(a + 0.9 * random_float(), 0.2, b + 0.9 * random_float());

			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				
				if (choose_mat < 0.8) {  
					// diffuse
			
					//point3 center2 = center + vec3(0, random_float(0, 0.5f), 0);

					world.add(make_shared<sphere>(center, 0.2, make_shared<lambertian>(
						color(random_float(), random_float(), random_float()))
					));
				}
				else if (choose_mat < 0.95) { 
					// metal
					
					world.add(make_shared<sphere>(center, 0.2,
						make_shared<metal>(vec3(0.5f * (1 + random_float()),
							0.5f * (1 + random_float()),
							0.5f * (1 + random_float())),
							0.5f * random_float())));
				}
				else {  
					// glass
					world.add(make_shared<sphere>(center, 0.2, make_shared<dielectric>(1.5)));
				}
			}
		}
	}

	world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, make_shared<dielectric>(1.5f)));
	world.add(make_shared<sphere>(vec3(-4, 1, 0), 1.0, make_shared<lambertian>(color(.5f, .5f, .5f))));
	world.add(make_shared<sphere>(vec3(4, 1, 0), 1.0, make_shared<metal>(vec3(0.7f, 0.6f, 0.5f), 0.0f)));

	return world;
}
