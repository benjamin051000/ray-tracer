#pragma once
#include "hittable_list.h"
#include "aabb.h"
#include "common.h"
#include <vector>

class bvh_node : public hittable {
public:
	bvh_node();

	bvh_node(hittable_list& list, float time0, float time1) :
		bvh_node(list.objects, 0, list.objects.size(), time0, time1)
	{}

	bvh_node(std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end, float  time0, float time1);

	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;
	virtual bool bounding_box(double t0, double t1, aabb& output_box) const override;

public:
	shared_ptr<hittable> left; // Can be bvh nodes or primitives
	shared_ptr<hittable> right;
	aabb box;
};


bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis);

bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b);
bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b);
bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b);

