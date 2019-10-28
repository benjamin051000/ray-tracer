#pragma once
#include "hittable.h"

struct sphere : public hittable {
	sphere() {}

	sphere(vec3 center, float radius) : center(center), radius(radius) {};
	vec3 center;
	float radius;

	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
};

bool sphere::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
	vec3 oc = r.origin() - center;

	//Calculate discriminant
	float a = dot(r.direction(), r.direction());
	float b = 2 * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a * c;

	if (discriminant > 0) {
		float t = (-b - sqrt(b * b - 4 * a * c)) / (2 * a); //why not over 2a?
		if (t < tmax && t > tmin) {
			rec.t = t;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			return true;
		}

		t = (-b + sqrt(b * b - a * c)) / (2 * a);
		if (t < tmax && t > tmin) { //refactor this into the first if statement?
			rec.t = t;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
	}
	return false;
}