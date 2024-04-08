#include "ray.hpp"

ray::ray() {}

ray::ray(const point3& origin, const vec3& direction, const double time) :
	origin(origin), direction(direction), time(time) {}

[[nodiscard]] point3 ray::at(const double t) const noexcept { return origin + t * direction; }

