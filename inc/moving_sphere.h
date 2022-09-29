#pragma once
#include "hittable.h"
#include "common.h"

class moving_sphere : public hittable {
public:

	moving_sphere() {}
	moving_sphere(point3 cen0, point3 cen1, double t0, double t1, float r, shared_ptr<material> m)
		: center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mp(m) {}

	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;

	virtual bool bounding_box(double _time0, double _time1, aabb& output_box) const override {
		aabb box0(
			center(_time0) - vec3(radius, radius, radius),
			center(_time0) + vec3(radius, radius, radius));
		aabb box1(
			center(_time1) - vec3(radius, radius, radius),
			center(_time1) + vec3(radius, radius, radius));
		output_box = surrounding_box(box0, box1);
		return true;
	}

	point3 center(double time) const;

private:
	point3 center0, center1;
	double time0 = 0, time1 = 0;
	double radius = 0;
	shared_ptr<material> mp;
};

bool moving_sphere::hit(const ray& r, double tmin, double tmax, hit_record& rec) const
{
	//Calculate discriminant
	/*2's and 4's have been commented out, as they cancel out
	in the grand scheme of things. However, they are helpful
	in understanding the math, so I will leave the comments in for now.*/
	vec3 oc = r.origin() - center(r.time());

	const auto a = dot(r.direction(), r.direction()),
		half_b = dot(oc, r.direction()), // Represents half of b, since it's normally x2.
		c = oc.length_squared() - radius * radius;
	
	const auto discriminant = half_b * half_b - /* 4 * */ a * c;

	auto sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	auto root = (-half_b - sqrtd) / a;
	if (root < tmin || tmax < root) {
		root = (-half_b + sqrtd) / a;
		if (root < tmin || tmax < root)
			return false;
	}

	rec.t = root;
	rec.p = r.at(rec.t);
	vec3 outward_normal = (rec.p - center(r.time())) / radius;
	rec.set_face_normal(r, outward_normal);
	rec.mp = mp;

	return true;
}

point3 moving_sphere::center(double time) const
{
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}
