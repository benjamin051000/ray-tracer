#pragma once
#include "hittable.h"
#include  "texture.h"


//ABC
class material {
public:
	virtual bool scatter(
		const ray& r_in, 
		const hit_record& rec, 
		vec3& attenuation, 
		ray& scattered) const = 0;
};


//A diffuse (matte) material.
class lambertian : public material {
public:
	lambertian() {}

	lambertian(shared_ptr<texture> a) : albedo(a) {}

	virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const {

		vec3 scatter_direction = rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, scatter_direction, r_in.time());
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		
		return true;
	}

private:
	shared_ptr<texture> albedo;
};


//TODO: Put this in metal class? If not used by anything else
vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n) * n;
}


//A metal with fuzz.
class metal : public material {
public:
	metal(const vec3& a, float f) : albedo(a) {
		fuzz = f < 1 ? f : 1;
	}
	virtual bool scatter(const ray& r_in, const hit_record& rec,
		vec3& attenuation, ray& scattered) const {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return dot(scattered.direction(), rec.normal) > 0;
	}

	//Reflected light (I guess?)
	vec3 albedo;
	//Fuzz value from 0 (shiny) to 1 (fuzzy).
	float fuzz;
};


vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
	float cos_theta = dot(-uv, n);
	vec3 r_out_parallel = etai_over_etat * (uv + cos_theta * n);
	vec3 r_out_perp = -sqrt(1. - r_out_parallel.length_squared()) * n;
	
	return r_out_parallel + r_out_perp;
}


//Approximation for varying reflectivity with angle.
float schlick(float cosine, float ref_idx) {
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 *= r0;
	return r0 + (1 - r0) * pow(1 - cosine, 5);
}


class dielectric : public material {
public:
	dielectric(float ri) : ref_idx(ri) {}

	virtual bool scatter(const ray& r_in, const hit_record& rec,
		color& attenuation, ray& scattered) const 
	{
		attenuation = color(1, 1, 1);
		float etai_over_etat = (rec.front_face) ? (1.f / ref_idx) : ref_idx;

		vec3 unit_direction = unit_vector(r_in.direction());

		float cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		float sin_theta = sqrt(1 - pow(cos_theta, 2));

		if (etai_over_etat * sin_theta > 1) {
			vec3 reflected = reflect(unit_direction, rec.normal);
			scattered = ray(rec.p, reflected);
			return true;
		}

		float reflect_prob = schlick(cos_theta, etai_over_etat);
		if (random_float() < reflect_prob) {
			vec3 reflected = reflect(unit_direction, rec.normal);
			scattered = ray(rec.p, reflected);
			return true;
		}

		vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
		scattered = ray(rec.p, refracted);
		return true;
	}

	//Refractive index
	float ref_idx;
};
