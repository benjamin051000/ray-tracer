#pragma once

#include "hittable_list.h"


enum class Scene {
	RANDOM_SCENE,
	TWO_SPHERES,
	TWO_PERLIN_SPHERES,
	EARTH,
	SIMPLE_LIGHT,
	CORNELL_BOX,
	CORNELL_SMOKE,
	FINAL_SCENE
};

hittable_list random_scene();
hittable_list two_spheres();
hittable_list two_perlin_spheres();
hittable_list earth();
hittable_list simple_light();
hittable_list cornell_box();
hittable_list cornell_smoke();
hittable_list final_scene();

color ray_color(const ray& r, const color& background, hittable& world, int depth);