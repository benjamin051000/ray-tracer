#pragma once
#include "common.h"
#include "hittable.h"

class sphere : public hittable {
public:
	sphere() {}
	sphere(point3 center, double radius, shared_ptr<material> material)
		: center(center), radius(radius), mp(material) {}

	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;

private:
	point3 center;
	double radius = 0;
	shared_ptr<material> mp;
	
	static void get_sphere_uv(const point3& p, double& u, double& v) {
		auto theta = acos(-p.y());
		auto phi = atan2(-p.z(), p.x()) + PI;

		u = phi / (2 * PI);
		v = theta / PI;
	}

	virtual bool bounding_box(double t0, double t1, aabb& output_box) const override;
};

