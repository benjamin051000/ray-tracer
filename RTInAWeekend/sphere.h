#pragma once
#include "hittable.h"

struct sphere : public hittable {
	sphere() {}

	sphere(vec3 center, float radius, material* material) : center(center), radius(radius), material(material) {}

	vec3 center;
	float radius;
	material* material;

	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
};

bool sphere::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
	vec3 oc = r.origin() - center;

	//Calculate discriminant
	/*2's and 4's have been commented out, as they cancel out
	in the grand scheme of things. However, they are helpful
	in understanding the math, so I will leave the comments in for now.*/
	const float a = dot(r.direction(), r.direction());
	const float b = /*2 * */ dot(oc, r.direction());
	const float c = dot(oc, oc) - radius * radius;
	const float discriminant = b * b - /* 4 * */ a * c;

	if (discriminant > 0) {
		//An intersection (or two) between r and the sphere exists.

		//Test closer intersection first
		float t = (-b - sqrt(b * b - /* 4 * */a * c)) / (/* 2 * */a);
		//Check if it's valid (within tmin and tmax)
		if (t < tmax && t > tmin) {
			rec.t = t;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.material = material;
			return true;
		}

		//If the closer intersection is invalid, try the farther one.
		t = (-b + sqrt(b * b - a * c)) / (/*2 **/ a);
		//Check validity
		if (t < tmax && t > tmin) {
			rec.t = t;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.material = material;
			return true;
		}
	}
	return false;
}