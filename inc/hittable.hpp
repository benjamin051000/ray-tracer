#pragma once
#include "common.hpp"
#include "aabb.hpp"

// Use a forward declaration for this class
// rather than `#include "material.hpp"` because
// the latter introduces a circular dependency
// (`material.hpp` relies on structs in this file)
// which causes weird issues. A forward declaration
// is just easier.
class material;


/*A record of a ray hitting an object.*/
struct hit_record {
	double t;
	point3 p;
	vec3 normal;
	// Material of the object.
	shared_ptr<material> mp;

	double u = 0, v = 0; // For texture mapping // TODO deprecate?

	bool front_face;

	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
		front_face = dot(r.direction, outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};


//ABC
struct hittable {
	virtual bool hit(
		const ray& r, 
		const double tmin, 
		const double tmax, 
		hit_record& rec) const = 0;

	virtual bool bounding_box(const double t0, const double t1, aabb& output_box) const = 0;
};


class translate : public hittable {
public:
    translate(shared_ptr<hittable> p, const vec3& displacement)
        : ptr(p), offset(displacement) {}

    virtual bool hit(
        const ray& r, 
		const double t_min, 
		const double t_max, 
		hit_record& rec
	) const override {
        const ray moved_r(r.origin - offset, r.direction, r.time);

        if (!ptr->hit(moved_r, t_min, t_max, rec))
            return false;

        rec.p += offset;
        rec.set_face_normal(moved_r, rec.normal);

        return true;
    }

    virtual bool bounding_box(const double time0, const double time1, aabb& output_box) const override {
        if (!ptr->bounding_box(time0, time1, output_box))
            return false;

        output_box = aabb(
            output_box.min() + offset,
            output_box.max() + offset
		);

        return true;
    }

public:
    shared_ptr<hittable> ptr;
    vec3 offset;
};


class rotate_y : public hittable {
public:
    rotate_y(shared_ptr<hittable> p, double angle) : ptr(p) {
        auto radians = deg_to_rad(angle);
        sin_theta = sin(radians);
        cos_theta = cos(radians);
        //hasbox = ptr->bounding_box(0, 1, bbox);

        point3 min(infinity, infinity, infinity);
        point3 max(-infinity, -infinity, -infinity);

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    auto x = i * bbox.max().x() + (1 - static_cast<double>(i)) * bbox.min().x();
                    auto y = j * bbox.max().y() + (1 - static_cast<double>(j)) * bbox.min().y();
                    auto z = k * bbox.max().z() + (1 - static_cast<double>(k)) * bbox.min().z();

                    auto newx = cos_theta * x + sin_theta * z;
                    auto newz = -sin_theta * x + cos_theta * z;

                    vec3 tester(newx, y, newz);

                    for (int c = 0; c < 3; c++) {
                        min[c] = fmin(min[c], tester[c]);
                        max[c] = fmax(max[c], tester[c]);
                    }
                }
            }
        }

        bbox = aabb(min, max);
    }

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
        auto origin = r.origin;
        auto direction = r.direction;

        origin[0] = cos_theta * r.origin[0] - sin_theta * r.origin[2];
        origin[2] = sin_theta * r.origin[0] + cos_theta * r.origin[2];

        direction[0] = cos_theta * r.direction[0] - sin_theta * r.direction[2];
        direction[2] = sin_theta * r.direction[0] + cos_theta * r.direction[2];

        ray rotated_r(origin, direction, r.time);

        if (!ptr->hit(rotated_r, t_min, t_max, rec))
            return false;

        auto p = rec.p;
        auto normal = rec.normal;

        p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
        p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];

        normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
        normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

        rec.p = p;
        rec.set_face_normal(rotated_r, normal);

        return true;
    }

    virtual bool bounding_box([[maybe_unused]] const double time0, [[maybe_unused]] const double time1, aabb& output_box) const override {
        output_box = bbox;
        return hasbox;
    }

public:
    shared_ptr<hittable> ptr;
    double sin_theta;
    double cos_theta;
    bool hasbox = false;
    aabb bbox;
};

