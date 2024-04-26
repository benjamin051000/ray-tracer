#include "moving_sphere.hpp"

bool moving_sphere::hit(const ray& r, double tmin, double tmax, hit_record& rec) const {
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

point3 moving_sphere::center(double time) const {
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

