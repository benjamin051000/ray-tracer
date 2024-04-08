#pragma once
#include "hittable.hpp"
#include  "texture.hpp"


// ABC
class material {
public:
	virtual bool scatter(
		const ray& r_in, 
		const hit_record& rec, 
		color& attenuation, 
		ray& scattered) const = 0;

	virtual color emitted([[maybe_unused]] double u, [[maybe_unused]] double v, [[maybe_unused]] const point3& p) const {
		return color(0, 0, 0);
	}
};


// A diffuse (matte) material.
class lambertian : public material {
public:
	lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}

	lambertian(shared_ptr<texture> a) : albedo(a) {}

	virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
	) const override {

		vec3 scatter_direction = rec.normal + random_unit_vector();

		// Catch degenerate scatter direction
		if (scatter_direction.near_zero()) {
			scatter_direction = rec.normal;
		}

		scattered = ray(rec.p, scatter_direction, r_in.time);
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		
		return true;
	}

private:
	shared_ptr<texture> albedo;
};


// A metal with fuzz.
class metal : public material {
public:
	metal(const vec3& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered
	) const override {

		vec3 reflected = reflect(unit_vector(r_in.direction), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time);
		attenuation = albedo;
		
		return dot(scattered.direction, rec.normal) > 0;
	}

	// Reflected light (I guess?)
	color albedo;
	// Fuzz value from 0 (shiny) to 1 (fuzzy).
	double fuzz;
};


class dielectric : public material {
public:
	dielectric(double index_of_refraction) : ref_idx(index_of_refraction) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec,	color& attenuation, ray& scattered
	) const override {
		attenuation = color(1.0, 1.0, 1.0);
		
		double refraction_ratio = rec.front_face ? (1.0 / ref_idx) : ref_idx;

		vec3 unit_direction = unit_vector(r_in.direction);

		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = refraction_ratio * sin_theta > 1.0;

		vec3 direction; // Called direction in the book

		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
			direction = reflect(unit_direction, rec.normal);	
		else
			direction = refract(unit_direction, rec.normal, refraction_ratio);

		scattered = ray(rec.p, direction);
		return true;
	}

	// Refractive index
	double ref_idx;

private:
	//Approximation for varying reflectivity with angle.
	// Uses Schlick's Approximation for reflectance.
	static double reflectance(double cosine, double ref_idx) {
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 *= r0;
		return r0 + (1 - r0) * pow(1 - cosine, 5);
	}
};


class diffuse_light : public material {
public:
	diffuse_light(shared_ptr<texture> a) : emit(a) {}
	diffuse_light(color c) : emit(make_shared<solid_color>(c)) {}

	virtual bool scatter(
		[[maybe_unused]] const ray& r_in, [[maybe_unused]] const hit_record& rec, [[maybe_unused]] color& attenuation, [[maybe_unused]] ray& scattered
	) const override {
		return false;
	}

	virtual color emitted(double u, double v, const point3& p) const override {
		return emit->value(u, v, p);
	}

public:
	shared_ptr<texture> emit;
};


class isotropic : public material {
public:
	isotropic(color c) : albedo(make_shared<solid_color>(c)) {}
	isotropic(shared_ptr<texture> a) : albedo(a) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override {
		scattered = ray(rec.p, random_in_unit_sphere(), r_in.time);
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}

public:
	shared_ptr<texture> albedo;
};
