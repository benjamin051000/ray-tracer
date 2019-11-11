#pragma once
#include "hittable.h"

struct hittable_list : public hittable {
	hittable_list() {}
	hittable_list(hittable** l, int n) { list = l; list_size = n; }

	//An array of hittable objects
	hittable** list;
	unsigned int list_size;

	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
};

bool hittable_list::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
	hit_record temp_rec;
	bool hit_anything = false;
	double closest_t = tmax;

	//For each hittable in the list
	for (unsigned int i = 0; i < list_size; i++) {
		//See if the ray hits anything in front of closest_t
		if (list[i]->hit(r, tmin, closest_t, temp_rec)) {
			hit_anything = true;
			closest_t = temp_rec.t;
		}
	}

	//Copy record information over
	rec = temp_rec;
	
	return hit_anything;
}