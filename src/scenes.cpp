#include "scenes.hpp"

#include "sphere.hpp"
#include "moving_sphere.hpp"
#include "material.hpp"
#include "aarect.hpp"
#include "box.hpp"
#include "constant_medium.hpp"
#include "bvh.hpp"

//Calculates the color of the pixel.
color ray_color(const ray& r, const color& background, hittable& world, int depth) {
	hit_record rec;

	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0) return color(0, 0, 0);

	// If the ray hits nothing, return background color.
	if (!world.hit(r, 0.001, infinity, rec))
		return background;

	ray scattered;
	color attenuation;
	color emitted = rec.mp->emitted(rec.u, rec.v, rec.p);

	if (!rec.mp->scatter(r, rec, attenuation, scattered))
		return emitted;

	return emitted + attenuation * ray_color(scattered, background, world, depth - 1);
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
	auto earth_texture = make_shared<image_texture>("earthmap.jpg");
	auto earth_surface = make_shared<lambertian>(earth_texture);
	auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

	return hittable_list(globe);
}


hittable_list simple_light() {
	hittable_list objects;

	auto pertext = make_shared<noise_texture>(4);
	objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
	objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

	auto difflight = make_shared<diffuse_light>(color(4, 4, 4));
	objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

	return objects;
}


hittable_list cornell_box() {
	hittable_list objects;

	auto red = make_shared<lambertian>(color(.65, .05, .05));
	auto white = make_shared<lambertian>(color(.73, .73, .73));
	auto green = make_shared<lambertian>(color(.12, .45, .15));
	auto light = make_shared<diffuse_light>(color(15, 15, 15));

	// Add walls and light
	objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
	objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
	objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
	objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
	objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

	objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));

	// Add two boxes to the scene
	shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
	box1 = make_shared<rotate_y>(box1, 15);
	box1 = make_shared<translate>(box1, vec3(265, 0, 295));
	objects.add(box1);

	shared_ptr<hittable> box2 = make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
	box2 = make_shared<rotate_y>(box2, -18);
	box2 = make_shared<translate>(box2, vec3(130, 0, 65));
	objects.add(box2);

	return objects;
}


hittable_list cornell_smoke() {
	hittable_list objects;

	auto red = make_shared<lambertian>(color(.65, .05, .05));
	auto white = make_shared<lambertian>(color(.73, .73, .73));
	auto green = make_shared<lambertian>(color(.12, .45, .15));
	auto light = make_shared<diffuse_light>(color(7, 7, 7));

	objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
	objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
	objects.add(make_shared<xz_rect>(113, 443, 127, 432, 554, light));
	objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
	objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
	objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

	shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
	box1 = make_shared<rotate_y>(box1, 15);
	box1 = make_shared<translate>(box1, vec3(265, 0, 295));

	shared_ptr<hittable> box2 = make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
	box2 = make_shared<rotate_y>(box2, -18);
	box2 = make_shared<translate>(box2, vec3(130, 0, 65));

	objects.add(make_shared<constant_medium>(box1, 0.01, color(0, 0, 0)));
	objects.add(make_shared<constant_medium>(box2, 0.01, color(1, 1, 1)));

	return objects;
}


hittable_list final_scene() {
	hittable_list boxes1;
	auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

	const int boxes_per_side = 20;
	for (int i = 0; i < boxes_per_side; i++) {
		for (int j = 0; j < boxes_per_side; j++) {
			auto w = 100.0;
			auto x0 = -1000.0 + i * w;
			auto z0 = -1000.0 + j * w;
			auto y0 = 0.0;
			auto x1 = x0 + w;
			auto y1 = random_double(1, 101);
			auto z1 = z0 + w;

			boxes1.add(make_shared<box>(point3(x0, y0, z0), point3(x1, y1, z1), ground));
		}
	}

	hittable_list objects;

	objects.add(make_shared<bvh_node>(boxes1, 0, 1));

	auto light = make_shared<diffuse_light>(color(7, 7, 7));
	objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));

	auto center1 = point3(400, 400, 200);
	auto center2 = center1 + vec3(30, 0, 0);
	auto moving_sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
	objects.add(make_shared<moving_sphere>(center1, center2, 0, 1, 50, moving_sphere_material));

	objects.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
	objects.add(make_shared<sphere>(
		point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
		));

	auto boundary = make_shared<sphere>(point3(360, 150, 145), 70, make_shared<dielectric>(1.5));
	objects.add(boundary);
	objects.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
	boundary = make_shared<sphere>(point3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
	objects.add(make_shared<constant_medium>(boundary, .0001, color(1, 1, 1)));

	auto emat = make_shared<lambertian>(make_shared<image_texture>("..\\assets\\earthmap.jpg"));
	objects.add(make_shared<sphere>(point3(400, 200, 400), 100, emat));
	auto pertext = make_shared<noise_texture>(0.1);
	objects.add(make_shared<sphere>(point3(220, 280, 300), 80, make_shared<lambertian>(pertext)));

	hittable_list boxes2;
	auto white = make_shared<lambertian>(color(.73, .73, .73));
	int ns = 1000;
	for (int j = 0; j < ns; j++) {
		boxes2.add(make_shared<sphere>(point3::random(0, 165), 10, white));
	}

	objects.add(make_shared<translate>(
		make_shared<rotate_y>(
			make_shared<bvh_node>(boxes2, 0.0, 1.0), 15),
		vec3(-100, 270, 395)
		)
	);

	return objects;
}

