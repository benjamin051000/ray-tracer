#pragma once
#include "ray.hpp"
#include <cmath>

struct aabb {
	point3 min, max;

	constexpr aabb() = default;
	constexpr aabb(const point3& a, const point3& b) : min(a), max(b) {}

	constexpr bool hit(const ray& r, double tmin, double tmax) const { // TODO override hittable?
		for (int a = 0; a < 3; a++) {
			const auto invD = 1.0f / r.direction[a];

			auto t0 = (min[a] - r.origin[a]) * invD;
			auto t1 = (max[a] - r.origin[a]) * invD;

			if (invD < 0.0f)
				std::swap(t0, t1);

			tmin = t0 > tmin ? t0 : tmin;
			tmax = t1 < tmax ? t1 : tmax;
			if (tmax <= tmin)
				return false;
		}
		return true;
	}
};


[[nodiscard]] constexpr aabb surrounding_box(const aabb box0, const aabb box1) {
	const point3 small(
		fmin(box0.min.x(), box1.min.x()),
		fmin(box0.min.y(), box1.min.y()),
		fmin(box0.min.z(), box1.min.z())
	);

	const point3 big(
		fmax(box0.max.x(), box1.max.x()),
		fmax(box0.max.y(), box1.max.y()),
		fmax(box0.max.z(), box1.max.z())
	);

	return aabb(small, big);
}
