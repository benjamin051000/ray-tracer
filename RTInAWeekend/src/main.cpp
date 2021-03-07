#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "common.h"

#include "hittable_list.h"
#include "camera.h"
#include "material.h"

#include "sphere.h"
#include "moving_sphere.h"

using std::chrono::steady_clock;
using std::chrono::duration_cast;

//Calculates the color of the pixel.
color ray_color(const ray &r, const color& background, hittable &world, int depth) {
	hit_record rec;
	
	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0) return color(0, 0, 0);

	// If the ray hits nothing, return background color.
	if (!world.hit(r, 0.001, infinity, rec))
		return background;

	ray scattered;
	color attenuation;
	color emitted = rec.material->emitted(rec.u, rec.v, rec.p);

	if (!rec.material->scatter(r, rec, attenuation, scattered))
		return emitted;

	return emitted + attenuation * ray_color(scattered, background, world, depth - 1);
}

//Calculates the color of the pixel.
//vec3 ray_color(const ray& r, const color& background, hittable& world, int depth) {
//	hit_record rec;
//
//	// If we've exceeded the ray bounce limit, no more light is gathered.
//	if (depth <= 0) return color(0, 0, 0);
//
//	if (world.hit(r, 0.001, infinity, rec)) {
//		ray scattered;
//		color attenuation;
//
//		if (rec.material->scatter(r, rec, attenuation, scattered))
//			return attenuation * ray_color(scattered, background, world, depth - 1);
//
//		return vec3(0, 0, 0);
//	}
//
//	//What is this block for? If the ray doesn't hit anything in the world, isn't the color just white?
//	vec3 unit_direction = unit_vector(r.direction());
//	float t = 0.5 * (unit_direction.y() + 1);
//	return (1 - t) * color(1, 1, 1) + t * color(0.5, 0.7, 1);
//}

hittable_list random_scene();
hittable_list two_spheres();
hittable_list two_perlin_spheres();
hittable_list earth();

int main() {
	/* Output image options */
	const unsigned int image_width = 640;
	const double aspect_ratio = 16.0 / 9.0;
	const int image_height = static_cast<int>(image_width / aspect_ratio);

	const unsigned int spp = 1; //Samples per pixel
	const int max_depth = 50; // Max recursive depth for ray reflections.
	
	/* Set up camera */
	point3 lookfrom(13,2,3), lookat(0,0,0);
	auto vfov = 40.0;
	auto aperture = 0.0;


	//Initialize the objects in the scene
	hittable_list world;
	
	// Used in the ray_color function.
	color background(0.70, 0.80, 1.00);


	/*----------Select Scene----------*/
	switch (3) {
	case 1:
		std::cout << "Scene 1 selected." << std::endl;
		world = random_scene();
		//lookfrom = point3(13, 2, 3);
		//lookat = point3(0, 0, 0);
		vfov = 20.0;
		aperture = 0.1;
		break;

	case 2:
		std::cout << "Scene 2 selected." << std::endl;
		world = two_spheres();
		//lookfrom = point3(13, 2, 13);
		//lookat = point3(0, 0, 0);
		vfov = 20.0;
		break;

	case 3:
		std::cout << "Scene 3 selected." << std::endl;
		world = two_perlin_spheres();
		//lookfrom = point3(13, 2, 3);
		//lookat = point3(0, 0, 0);
		vfov = 20.0;
		break;

	case 4:
		std::cout << "Scene 4 selected." << std::endl;
		world = earth();
		//lookfrom = point3(13, 2, 3);
		//lookat = point3(0, 0, 0);
		vfov = 20.0;
		break;

	case 5:
		std::cout << "Scene 5 selected." << std::endl;
		background = color(0.0, 0.0, 0.0);
		break;

	default:
		std::cout << "You must select a non-zero number.";
		exit(1);
	}

	/*----------Set up Camera----------*/
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	camera cam(lookfrom, lookat, vup, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

	// Array to hold pixel colors (x * y * num_pixels)
	unsigned char* pixels = new unsigned char[image_width * image_height * 3];
	unsigned int pixel_idx = 0;

	/*----------Render the image----------*/
	double percentage = 0;
	const double percentChange = 100.0 / image_height ;

	auto start = steady_clock::now();

	// Main loop
	for (int y = image_height - 1; y >= 0; y--) {

		auto row_start = steady_clock::now();

		for (unsigned int x = 0; x < image_width; x++) {
			color col(0, 0, 0);
			//Repeat samples-per-pixel times
			for (unsigned int s = 0; s < spp; s++) {

				//Get a random ray in the pixel
				float u = float(x + random_double()) / float(image_width);
				float  v = float(y + random_double()) / float(image_height);

				ray r = cam.get_ray(u, v);
				col += ray_color(r, background, world, max_depth);
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

	auto total_seconds = duration_cast<std::chrono::seconds>(stop - start).count();
	std::cout << "Total render time: " << total_seconds / 60 << " min" << total_seconds % 60 << " sec" << std::endl;

	//Set up the output file
	const std::string filename = "..\\render_" + std::to_string(image_width) + "_" + std::to_string(image_height) + "_" + std::to_string(spp) + ".jpg";
	stbi_write_jpg(filename.c_str(), image_width, image_height, 3, pixels, 100);

}

hittable_list random_scene() {
	hittable_list world;

	// Creates the "ground" (a large sphere beneath the rest).
	world.add(make_shared<sphere>(
		vec3(0, -1000, 0), 
		1000, 
		make_shared<lambertian>(make_shared<solid_color>(0.5, 0.5, 0.5))
	));

	// Creates 484 spheres of varying materials and positions.
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {

			float choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				
				if (choose_mat < 0.8) {  
					// diffuse
			
					//point3 center2 = center + vec3(0, random_double(0, 0.5f), 0);

					world.add(make_shared<sphere>(center, 0.2, make_shared<lambertian>(
						make_shared<solid_color>(random_double(), random_double(), random_double()))
					));
				}
				else if (choose_mat < 0.95) { 
					// metal
					
					world.add(make_shared<sphere>(center, 0.2,
						make_shared<metal>(vec3(0.5f * (1 + random_double()),
							0.5f * (1 + random_double()),
							0.5f * (1 + random_double())),
							0.5f * random_double())));
				}
				else {  
					// glass
					world.add(make_shared<sphere>(center, 0.2, make_shared<dielectric>(1.5)));
				}
			}
		}
	}

	world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, make_shared<dielectric>(1.5f)));
	world.add(make_shared<sphere>(vec3(-4, 1, 0), 1.0, make_shared<lambertian>(make_shared<solid_color>(.5f, .5f, .5f))));
	world.add(make_shared<sphere>(vec3(4, 1, 0), 1.0, make_shared<metal>(vec3(0.7f, 0.6f, 0.5f), 0.0f)));

	return world;
}

hittable_list two_spheres() {
	hittable_list objects;

	auto checker = make_shared<checker_texture>(
		make_shared <solid_color>(0.2, 0.3, 0.1),
		make_shared<solid_color>(0.9, 0.9, 0.9)
	);

	objects.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
	objects.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));
	return objects;
}

hittable_list two_perlin_spheres() {
	hittable_list objects;

	auto pertext = make_shared<noise_texture>(4);
	objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
	objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));
	return objects;
}

hittable_list earth() {
	auto earth_texture = make_shared<image_texture>("..\\earthmap.jpg");
	auto earth_surface = make_shared<lambertian>(earth_texture);
	auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

	return hittable_list(globe);
}