#include "vec3.hpp"
#include "common.hpp"

// Static methods
[[nodiscard]] vec3 vec3::random() {
	return vec3(random_double(), random_double(), random_double());
}

[[nodiscard]] vec3 vec3::random(const double min, const double max) {
	return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

std::ostream& operator<<(std::ostream& output, vec3& t) {
	return output << t[0] << " " << t[1] << " " << t[2];
}

/*----------Random Vector Generators----------*/
[[nodiscard]] vec3 random_in_unit_disk() {
	while (true) {
		vec3 p(random_double(-1, 1), random_double(-1, 1), 0);

		if (p.length_squared() < 1)  return p;
	}
}

[[nodiscard]] vec3 random_in_unit_sphere() {
	while (true) {
		vec3 p = vec3::random(-1, 1);

		if (p.length_squared() < 1) return p;
	}
}

[[nodiscard]] vec3 random_unit_vector() {
	return unit_vector(random_in_unit_sphere());
}
