#pragma once
#include "common.h"
#include "hittable.h"

class sphere : public hittable {
public:
	sphere() {}
	sphere(point3 center, double radius, shared_ptr<material> material)
		: center(center), radius(radius), material(material) {}

	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;

private:
	point3 center;
	double radius;
	shared_ptr<material> material;
	
	static void get_sphere_uv(const point3& p, double& u, double& v) {
		auto theta = acos(-p.y());
		auto phi = atan2(-p.z(), p.x()) + PI;

		u = phi / (2 * PI);
		v = theta / PI;
	}

	virtual bool bounding_box(double t0, double t1, aabb& output_box) const override;
};

bool sphere::bounding_box(double t0, double t1, aabb& output_box) const {
	output_box = aabb(
		center - vec3(radius, radius, radius),
		center + vec3(radius, radius, radius));
	return true;
}

bool sphere::hit(const ray& r, double tmin, double tmax, hit_record& rec) const {
	vec3 oc = r.origin() - center;

	//Calculate discriminant
	/*2's and 4's have been commented out, as they cancel out
	in the grand scheme of things. However, they are helpful
	in understanding the math, so I will leave the comments in for now.*/
	const float a = r.direction().length_squared(),
		half_b = dot(oc, r.direction()), // Represents half of b, since it's normally *2.
		c = oc.length_squared() - radius * radius;

	const float discriminant = half_b * half_b - /* 4 * */ a * c;
	if (discriminant < 0) return false;


	float sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	auto root = (-half_b - sqrtd) / a;

	if (root < tmin || root > tmax) {
		root = (-half_b + sqrtd) / a;
		if (root < tmin || root > tmax)
			return false;
	}

	rec.t = root;
	rec.p = r.at(rec.t);
	vec3 outward_normal = (rec.p - center) / radius;
	rec.set_face_normal(r, outward_normal);
	get_sphere_uv(outward_normal, rec.u, rec.v);
	rec.material = material;

	return true;
}


