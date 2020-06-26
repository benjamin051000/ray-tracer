#pragma once
#include "hittable.h"
#include "common.h"

class moving_sphere : public hittable {
public:

	moving_sphere() {}
	moving_sphere(point3 cen0, point3 cen1, double t0, double t1, float r, shared_ptr<material> m)
		: center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m) {};

	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;

	point3 center(double time) const;

private:
	point3 center0, center1;
	double time0 = 0, time1 = 0;
	float radius = 0;
	shared_ptr<material> mat_ptr;
};

bool moving_sphere::hit(const ray& r, float tmin, float tmax, hit_record& rec) const
{
	//Calculate discriminant
	/*2's and 4's have been commented out, as they cancel out
	in the grand scheme of things. However, they are helpful
	in understanding the math, so I will leave the comments in for now.*/
	vec3 oc = r.origin() - center(r.time());

	const float a = dot(r.direction(), r.direction());
	const float half_b = dot(oc, r.direction()); // Represents half of b, since it's normally x2.
	const float c = oc.length_squared() - radius * radius;
	const float discriminant = half_b * half_b - /* 4 * */ a * c;

	if (discriminant > 0) {
		// An intersection (or two) between r and the sphere exists.

		float root = sqrt(discriminant);

		// Test closer intersection first
		float temp = (-half_b - root) / a; // a normally x2
		// Check if it's valid (within tmin and tmax)
		if (temp < tmax && temp > tmin) {
			rec.t = temp;
			rec.p = r.at(rec.t);

			vec3 outward_normal = (rec.p - center(r.time())) / radius;
			rec.set_face_normal(r, outward_normal);

			rec.material = mat_ptr;
			return true;
		}

		// If the closer intersection is invalid, try the farther one.
		temp = (-half_b + root) / a;
		// Check validity
		if (temp < tmax && temp > tmin) {
			rec.t = temp;
			rec.p = r.at(rec.t);

			vec3 outward_normal = (rec.p - center(r.time())) / radius;
			rec.set_face_normal(r, outward_normal);

			rec.material = mat_ptr;
			return true;
		}
	}
	return false;
}

point3 moving_sphere::center(double time) const
{
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}