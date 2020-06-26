#pragma once
#include "hittable.h"

#include "common.h"

#include <vector>


struct hittable_list : public hittable {
	hittable_list() {}
	hittable_list(shared_ptr<hittable> object) { add(object); }

	void clear() { objects.clear(); }
	void add(shared_ptr<hittable> object) { objects.push_back(object); }
	
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;

	//bool bounding_box(float t0, float t1, aabb& output_box) const;

	//An array of hittable objects
	std::vector<shared_ptr<hittable>> objects;
};

//bool hittable_list::bounding_box(float t0, float t1, aabb& output_box) const {
//	if (objects.empty()) return false;
//
//	aabb temp_box;
//	bool first_box = true;
//
//	for (const auto& object : objects) {
//		if (!object->bounding_box(t0, t1, temp_box)) return false;
//		output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
//		first_box = false;
//	}
//
//	return true;
//}


bool hittable_list::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
	hit_record temp_rec;
	bool hit_anything = false;
	float closest_so_far = tmax;

	//For each hittable in the list
	for (const auto& object : objects) {
		//See if the ray hits anything in front of closest_t
		if (object->hit(r, tmin, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
		}
	}

	//Copy record information over
	rec = temp_rec;
	
	return hit_anything;
}