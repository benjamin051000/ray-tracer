#pragma once
#include "common.h"

class aabb {
public:
	aabb() {}
	aabb(const point3& a, const point3& b) { _min = a; _max = b; }

	point3 min() const { return _min; }
	point3 max() const { return _max; }

	bool hit(const ray& r, double tmin, double tmax) const;

	point3 _min, _max;
};


aabb surrounding_box(aabb box0, aabb box1);