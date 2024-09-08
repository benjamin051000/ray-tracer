#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include <unordered_map>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "common.hpp"

#include "hittable_list.hpp"

#include "scenes.hpp"

#include "camera.hpp"

using std::chrono::steady_clock;
using std::chrono::duration_cast;


namespace {

void run(
	unsigned int specified_start_width,
	unsigned int specified_start_height,
	unsigned int specified_end_width,
	unsigned int specified_end_height,
	unsigned int image_width,
	unsigned int image_height,
	unsigned int spp,
	const camera& cam,
	color& background,
	hittable_list& world,
	int max_depth,
	std::vector<unsigned char>& pixels
) {
	/*----------Render the image----------*/
	double percentage = 0;
	const double percentChange = 100.0 / specified_end_height;

	const auto start = steady_clock::now();

	// Main loop
	for (unsigned y = specified_start_height; y < specified_end_height; y++) {

		const auto row_start = steady_clock::now();

		for (unsigned x = specified_start_width; x < specified_end_width; x++) {
			color pixel_color(0, 0, 0);
			//Repeat samples-per-pixel times
			for (unsigned s = 0; s < spp; s++) {

				//Get a random ray in the pixel
				const float u = float(x + random_double()) / float(image_width);
				const float  v = float(y + random_double()) / float(image_height);

				const ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, background, world, max_depth);
			}

			//Average the color value
			pixel_color /= float(spp);
			//Not sure why but square root the colors
			pixel_color = vec3(sqrt(pixel_color[0]), sqrt(pixel_color[1]), sqrt(pixel_color[2]));

			//Write the rgb values
			const unsigned char ir = static_cast<unsigned char>(255 * pixel_color[0]),
				ig = static_cast<unsigned char>(255 * pixel_color[1]),
				ib = static_cast<unsigned char>(255 * pixel_color[2]);

			const auto index = 3 * image_width * (image_height - 1 - y) + 3 * x;
			pixels[index] = ir;
			pixels[index + 1] = ig;
			pixels[index + 2] = ib;
		}

		const auto row_done = steady_clock::now();
		percentage += percentChange;
		const auto elapsed_row = duration_cast<std::chrono::seconds>(row_done - row_start).count();

		std::cout << "[" << std::this_thread::get_id() << "] " << percentage << "% complete(Row time : " << elapsed_row << " s)\n";

		/*auto est_time_remaining = (elapsed_row / percentage) * (100 - percentage);
		std::cout << "\tEst. time remaining: " << est_time_remaining << " s" << std::endl;*/
	}

	const auto stop = steady_clock::now();

	const auto total_seconds = duration_cast<std::chrono::seconds>(stop - start).count();
	std::cout << "[" << std::this_thread::get_id() << "] Total render time: " << total_seconds / 60 << " min " << total_seconds % 60 << " sec" << std::endl;

}

struct options {
    Scene scene;
    unsigned int width, height, spp, jobs;
	float aspect_ratio;
};

options parse_args(int argc, char** argv) {
    const int NUM_REQUIRED_ARGS = 5;

    if (argc < NUM_REQUIRED_ARGS) {
        std::cout << 
            "Usage:\n"
            "\traytracer <scene number> <image width> <image height> <samples per pixel> <num jobs>\n\n"
			"(Aspect ratio is calculated automatically)\n"
			"\n"
            "Scene numbers:\n"
            "1. Random scene (Cover of book 1)\n"
            "2. Two spheres\n"
            "3. Two perlin spheres\n"
            "4. Earth\n"
            "5. Simple light\n"
            "6. Cornell box\n"
            "7. Cornell smoke\n"
            "8. Final scene (Cover of book 2)\n";

        exit(EXIT_FAILURE);
    }

    const std::vector<std::string> args(argv + 1, argv + argc);

    for(const auto& arg: args) {
        std::cout << '"' << arg << '"' << "\n";
    }

    options opts;
    // First arg is the scene
    const int num = std::stoi(args.at(0));

    const std::unordered_map<int, Scene> scenes = {
        {1, Scene::RANDOM_SCENE},
        {2, Scene::TWO_SPHERES},
        {3, Scene::TWO_PERLIN_SPHERES},
        {4, Scene::EARTH},
        {5, Scene::SIMPLE_LIGHT},
        {6, Scene::CORNELL_BOX},
        {7, Scene::CORNELL_SMOKE},
        {8, Scene::FINAL_SCENE},
    };

    opts.scene = scenes.at(num);

    // Image width, height
    opts.width = static_cast<unsigned int>(std::stoi(args.at(1)));
    opts.height = static_cast<unsigned int>(std::stoi(args.at(2)));
    // spp
    opts.spp = static_cast<unsigned int>(std::stoi(args.at(3)));
    // num jobs
    opts.jobs = static_cast<unsigned int>(std::stoi(args.at(4)));

	opts.aspect_ratio = static_cast<float>(opts.height) / static_cast<float>(opts.width);
	std::cout << "aspect_ratio=" << opts.aspect_ratio << "\n";

    return opts;
}

} // end of anonymous namespace

int main(int argc, char** argv) {

    const auto opts = parse_args(argc, argv);
	/* Output image options */
	// auto opts.width = 1200;
	// auto aspect_ratio = 4.0 / 3.0;
	//auto opts.height = static_cast<int>(opts.width / aspect_ratio);
	// auto opts.height = 900;

	// const auto opts.spp = 50; // Samples per pixel
	const auto MAX_DEPTH = 50; // Max recursive depth for ray reflections.
	
    /* Number of worker threads to render the scene. */
	// const unsigned opts.jobs = 16;
	// const Scene SCENE = Scene::FINAL_SCENE;
	
	//////////////////////////////////////
	// Default values for each scene (may be overridden by scene switch stmt)
	//////////////////////////////////////
	/* Set up camera */
	point3 lookfrom, lookat;
	auto vfov = 40.0;
	auto aperture = 0.0;

	//Initialize the objects in the scene
	hittable_list world;
	
	// Color of the "skybox"/background (when ray hits nothing)
	color background(0.70, 0.80, 1.00);

	/*----------Select Scene----------*/
	switch (opts.scene) {
	case Scene::RANDOM_SCENE:
		world = random_scene();
		lookfrom = point3(13, 2, 3);
		lookat = point3(0, 0, 0);
		vfov = 20.0;
		aperture = 0.1;
		break;

	case Scene::TWO_SPHERES:
		world = two_spheres();
		lookfrom = point3(13, 2, 13);
		lookat = point3(0, 0, 0);
		vfov = 20.0;
		break;

	case Scene::TWO_PERLIN_SPHERES:
		world = two_perlin_spheres();
		lookfrom = point3(13, 2, 3);
		lookat = point3(0, 0, 0);
		vfov = 20.0;
		break;

	case Scene::EARTH:
		world = earth();
		lookfrom = point3(13, 2, 3);
		lookat = point3(0, 0, 0);
		vfov = 20.0;
		break;

	case Scene::SIMPLE_LIGHT:
		world = simple_light();
		background = color(0.0, 0.0, 0.0);
		lookfrom = point3(26, 3, 26);
		lookat = point3(0, 2, 0);
		vfov = 20;
		break;

	case Scene::CORNELL_BOX:
		world = cornell_box();
		// aspect_ratio = 1;
		// opts.width = 800;
		// opts.height = 800;
		background = color(0, 0, 0);
		lookfrom = point3(278, 278, -800);
		lookat = point3(278, 278, 0);
		vfov = 40;
		break;

	case Scene::CORNELL_SMOKE:
		world = cornell_smoke();
		// aspect_ratio = 1;
		// opts.width = 600;
		// opts.height = 600;
		lookfrom = point3(278, 278, -800);
		lookat = point3(278, 278, 0);
		vfov = 40;
		break;

	case Scene::FINAL_SCENE:
		world = final_scene();
		// aspect_ratio = 4.0/3.0;
		// opts.width = 640;
		// opts.height = 480;
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
	const vec3 vup(0, 1, 0);
	const auto dist_to_focus = 10.0;
	const camera cam(lookfrom, lookat, vup, vfov, opts.aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

	// Array to hold pixel colors (x * y * num_pixels)
	const unsigned int IMG_NUM_PIXELS = opts.width * opts.height * 3U;
	std::vector<unsigned char> pixels(IMG_NUM_PIXELS);

	std::vector<std::thread> threads(opts.jobs);

	auto width_incr = opts.width / opts.jobs;

	for (unsigned int i = 0; i < opts.jobs; i++) {
		const auto start = i * width_incr;
		const auto end = (i + 1) * width_incr;

		// capture start,end by value, since their value changes each iteration.
		threads[i] = std::thread([&, start, end] {
			std::cout << "[" << std::this_thread::get_id() << "] start: " << start << ", end:" << end << std::endl;
			run(
				start,
				0,
				end,
				opts.height,
				opts.width,
				opts.height,
				opts.spp,
				cam,
				background,
				world,
				MAX_DEPTH,
				pixels
			);
		});
	}

	std::cout << opts.jobs << " threads started." << std::endl;
	
	// Wait for threads to finish.
	for (std::thread& t : threads) {
		t.join();
	}

	std::cout << "Threads finished!" << std::endl;

	//Set up the output file
	const std::string filename = "render_" + std::to_string(opts.width) + "_" + std::to_string(opts.height) + "_" + std::to_string(opts.spp) + ".jpg";
	stbi_write_jpg(filename.c_str(), static_cast<int>(opts.width), static_cast<int>(opts.height), 3, pixels.data(), 100);
	std::cout << "Done." << std::endl;

	// delete[] pixels;  // Free memory
}

