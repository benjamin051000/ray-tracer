#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <chrono>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "common.h"

#include "hittable_list.h"

#include "scenes.h"

#include "camera.h"

using std::chrono::steady_clock;
using std::chrono::duration_cast;


void run(
	unsigned int specified_start_width,
	unsigned int specified_start_height,
	unsigned int specified_end_width,
	unsigned int specified_end_height,
	unsigned int image_width,
	unsigned int image_height,
	unsigned int spp,
	camera& cam,
	color& background,
	hittable_list& world,
	int max_depth,
	unsigned char* pixels
) {
	/*----------Render the image----------*/
	double percentage = 0;
	const double percentChange = 100.0 / specified_end_height;

	auto start = steady_clock::now();

	// Main loop
	for (unsigned int y = specified_start_height; y < specified_end_height; y++) {

		auto row_start = steady_clock::now();

		for (unsigned int x = specified_start_width; x < specified_end_width; x++) {
			color pixel_color(0, 0, 0);
			//Repeat samples-per-pixel times
			for (unsigned int s = 0; s < spp; s++) {

				//Get a random ray in the pixel
				float u = float(x + random_double()) / float(image_width);
				float  v = float(y + random_double()) / float(image_height);

				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, background, world, max_depth);
			}

			//Average the color value
			pixel_color /= float(spp);
			//Not sure why but square root the colors
			pixel_color = vec3(sqrt(pixel_color[0]), sqrt(pixel_color[1]), sqrt(pixel_color[2]));

			//Write the rgb values
			unsigned char ir = int(255 * pixel_color[0]),
				ig = int(255 * pixel_color[1]),
				ib = int(255 * pixel_color[2]);

			auto index = 3 * image_width * (image_height - 1 - y) + 3 * x;
			pixels[index] = ir;
			pixels[index + 1] = ig;
			pixels[index + 2] = ib;
		}

		auto row_done = steady_clock::now();
		percentage += percentChange;
		auto elapsed_row = duration_cast<std::chrono::seconds>(row_done - row_start).count();

		std::cout << "[" << std::this_thread::get_id() << "] " << percentage << "% complete(Row time : " << elapsed_row << " s)\n";

		/*auto est_time_remaining = (elapsed_row / percentage) * (100 - percentage);
		std::cout << "\tEst. time remaining: " << est_time_remaining << " s" << std::endl;*/
	}

	auto stop = steady_clock::now();

	auto total_seconds = duration_cast<std::chrono::seconds>(stop - start).count();
	std::cout << "[" << std::this_thread::get_id() << "] Total render time: " << total_seconds / 60 << " min " << total_seconds % 60 << " sec" << std::endl;

}


int main() {
	/* Output image options */
	unsigned int image_width = 640;
	double aspect_ratio = 4.0 / 3.0; //16.0 / 9.0;
	//unsigned int image_height = static_cast<int>(image_width / aspect_ratio);
	unsigned int image_height = 480;

	unsigned int spp = 100; //Samples per pixel
	const int max_depth = 50; // Max recursive depth for ray reflections.
	
	/* Set up camera */
	point3 lookfrom(13,2,3), lookat(0,0,0);
	auto vfov = 40.0;
	auto aperture = 0.0;

	/* Number of worker threads to render the scene. */
	const unsigned num_threads = 8;
	const Scene scene = Scene::RANDOM_SCENE;

	//Initialize the objects in the scene
	hittable_list world;
	
	// Used in the ray_color function.
	color background(0.70, 0.80, 1.00);


	/*----------Select Scene----------*/
	switch (scene) {
	case Scene::RANDOM_SCENE:
		std::cout << "Scene 1 selected." << std::endl;
		world = random_scene();
		//lookfrom = point3(13, 2, 3);
		//lookat = point3(0, 0, 0);
		vfov = 20.0;
		aperture = 0.1;
		break;

	case Scene::TWO_SPHERES:
		std::cout << "Scene 2 selected." << std::endl;
		world = two_spheres();
		//lookfrom = point3(13, 2, 13);
		//lookat = point3(0, 0, 0);
		vfov = 20.0;
		break;

	case Scene::TWO_PERLIN_SPHERES:
		std::cout << "Scene 3 selected." << std::endl;
		world = two_perlin_spheres();
		//lookfrom = point3(13, 2, 3);
		//lookat = point3(0, 0, 0);
		vfov = 20.0;
		break;

	case Scene::EARTH:
		std::cout << "Scene 4 selected." << std::endl;
		world = earth();
		//lookfrom = point3(13, 2, 3);
		//lookat = point3(0, 0, 0);
		vfov = 20.0;
		break;

	case Scene::SIMPLE_LIGHT:
		std::cout << "Scene 5 selected." << std::endl;
		world = simple_light();
		background = color(0.0, 0.0, 0.0);
		lookfrom = point3(26, 3, 26);
		lookat = point3(0, 2, 0);
		vfov = 20;
		break;

	case Scene::CORNELL_BOX:
		world = cornell_box();
		aspect_ratio = 1;
		image_width = 800;
		image_height = 800;
		background = color(0, 0, 0);
		spp = 5000;
		lookfrom = point3(278, 278, -800);
		lookat = point3(278, 278, 0);
		vfov = 40;
		break;

	case Scene::CORNELL_SMOKE:
		world = cornell_smoke();
		aspect_ratio = 1;
		image_width = 600;
		image_height = 600;
		spp = 200;
		lookfrom = point3(278, 278, -800);
		lookat = point3(278, 278, 0);
		vfov = 40;
		break;

	case Scene::FINAL_SCENE:
		world = final_scene();
		aspect_ratio = 4.0/3.0;
		image_width = 640;
		image_height = 480;
		spp = 50;
		background = color(0, 0, 0);
		lookfrom = point3(478, 278, -600);
		lookat = point3(278, 278, 0);
		vfov = 40.0;
		break;

	default:
		std::cout << "You must select a non-zero number.";
		exit(1);
	}

	/*----------Set up Camera----------*/
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

	// Array to hold pixel colors (x * y * num_pixels)
	const unsigned int IMG_NUM_PIXELS = image_width * image_height * 3U;
	unsigned char* pixels = new unsigned char[IMG_NUM_PIXELS];

	std::vector<std::thread> threads(num_threads);

	auto width_incr = image_width / num_threads;

	for (unsigned int i = 0; i < num_threads; i++) {
		const auto start = i * width_incr;
		const auto end = (i + 1) * width_incr;

		// capture start,end by value, since their value changes each iteration.
		threads[i] = std::thread([&, start, end] {
			std::cout << "[" << std::this_thread::get_id() << "] start: " << start << ", end:" << end << std::endl;
			run(
				start,
				0,
				end,
				image_height,
				image_width,
				image_height,
				spp,
				cam,
				background,
				world,
				max_depth,
				pixels
			);
		});
	}

	std::cout << num_threads << " threads started." << std::endl;
	
	// Wait for threads to finish.
	for (std::thread& t : threads) {
		t.join();
	}

	std::cout << "Threads finished!" << std::endl;

	//Set up the output file
	const std::string filename = "..\\render_" + std::to_string(image_width) + "_" + std::to_string(image_height) + "_" + std::to_string(spp) + ".jpg";
	stbi_write_jpg(filename.c_str(), image_width, image_height, 3, pixels, 100);
	std::cout << "Done." << std::endl;

	delete[] pixels;  // Free memory
}

