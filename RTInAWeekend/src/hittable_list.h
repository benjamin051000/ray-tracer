#pragma once
#include "hittable.h"
#include "common.h"

#include <vector>


struct hittable_list : public hittable {
	hittable_list() {}
	hittable_list(shared_ptr<hittable> object) { add(object); }

	void clear() { objects.clear(); }
	void add(shared_ptr<hittable> object) { objects.push_back(object); }
	
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;

	virtual bool bounding_box(double t0, double t1, aabb& output_box) const override;

	//An array of hittable objects
	std::vector<shared_ptr<hittable>> objects;
};
