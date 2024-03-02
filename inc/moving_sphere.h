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

