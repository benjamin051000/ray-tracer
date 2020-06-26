#pragma once
#include "common.h"
#include "perlin.h"

class texture {
public:
	virtual color value(float u, float v, const point3& p) const = 0;
};

class solid_color : public texture {
public:
	solid_color() {}
	solid_color(color c) : color_value(c) {}
	solid_color(float r, float g, float b) : solid_color(color(r, g, b)) {}

	virtual color value(float u, float v, const vec3& p) const {
		return color_value;
	}

private:
	color color_value;
};

class checker_texture : public texture {
public:
	checker_texture() {}
	checker_texture(shared_ptr<texture> t0, shared_ptr<texture> t1) : even(t0), odd(t1) {}

	virtual color value(float u, float v, const point3& p) const {
		auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
		if (sines < 0)
			return odd->value(u, v, p);
		else
			return even->value(u, v, p);
	}

public:
	shared_ptr<texture> odd, even;
};

class noise_texture : public texture {
public:
	noise_texture() {}
	noise_texture(float sc) : scale(sc) {}

	virtual color value(float u, float v, const point3& p) const {
		return color(1, 1, 1) * noise.noise(scale * p);
	}

public:
	perlin noise;
	float scale;
};
