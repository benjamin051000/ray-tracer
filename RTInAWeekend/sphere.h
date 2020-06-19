#pragma once
#include "common.h"
#include "hittable.h"

class sphere : public hittable {
public:
	sphere() {}
	sphere(point3 center, float radius, shared_ptr<material> material) : center(center), radius(radius), material(material) {}

private:
	point3 center;
	float radius = 0;
	shared_ptr<material> material;

	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;

	//virtual bool bounding_box(float t0, float t1, aabb& output_box) const;
};

//bool sphere::bounding_box(float t0, float t1, aabb& output_box) const {
//	output_box = aabb(
//		center - vec3(radius, radius, radius),
//		center + vec3(radius, radius, radius));
//	return true;
//}

bool sphere::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
	vec3 oc = r.origin() - center;

	//Calculate discriminant
	/*2's and 4's have been commented out, as they cancel out
	in the grand scheme of things. However, they are helpful
	in understanding the math, so I will leave the comments in for now.*/
	const float a = dot(r.direction(), r.direction());
	const float half_b = dot(oc, r.direction()); // Represents half of b, since it's normally x2.
	const float c = dot(oc, oc) - radius * radius;
	const float discriminant = half_b * half_b - /* 4 * */ a * c;

	if (discriminant > 0) {
		//An intersection (or two) between r and the sphere exists.

		float root = sqrt(discriminant);

		//Test closer intersection first
		float temp = (-half_b - root) / a; // a normally x2
		//Check if it's valid (within tmin and tmax)
		if (temp < tmax && temp > tmin) {
			rec.t = temp;
			rec.p = r.at(rec.t);

			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(r, outward_normal);

			rec.material = material;
			return true;
		}

		//If the closer intersection is invalid, try the farther one.
		temp = (-half_b + root) / a;
		//Check validity
		if (temp < tmax && temp > tmin) {
			rec.t = temp;
			rec.p = r.at(rec.t);

			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(r, outward_normal);

			rec.material = material;
			return true;
		}
	}
	return false;
}

void get_sphere_uv(const vec3& p, double& u, double& v) {
	auto phi = atan2(p.z(), p.x());
	auto theta = asin(p.y());
	u = 1 - (phi + PI) / (2 * PI);
	v = (theta + PI / 2) / PI;
}
