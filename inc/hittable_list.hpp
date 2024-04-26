#pragma once
#include "hittable.hpp"
#include "common.hpp"

#include <vector>


struct hittable_list : public hittable {
	std::vector<shared_ptr<hittable>> objects;

	hittable_list() = default;
	hittable_list(const shared_ptr<hittable> object) { add(object); }

	void clear() { objects.clear(); }
	void add(const shared_ptr<hittable> object) { objects.push_back(object); }
	
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;

	virtual bool bounding_box(double t0, double t1, aabb& output_box) const override;

};
