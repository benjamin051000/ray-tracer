#include "vec3.hpp"

#include "common.hpp"

// Constructors
vec3::vec3() : e{ 0, 0, 0 } {}

vec3::vec3(const double e0, const double e1, const double e2) : e{ e0, e1, e2 } {}

[[nodiscard]] vec3 vec3::operator-() const { 
	return vec3(-e[0], -e[1], -e[2]); 
}

/*-----Assignment operators-----*/
vec3& vec3::operator+=(const vec3& rhs) {
	e[0] += rhs[0];
	e[1] += rhs[1];
	e[2] += rhs[2];
	return *this;
}

vec3& vec3::operator-=(const vec3& rhs) {
	e[0] -= rhs[0];
	e[1] -= rhs[1];
	e[2] -= rhs[2];
	return *this;
}

vec3& vec3::operator*=(const vec3& rhs) {
	e[0] *= rhs[0];
	e[1] *= rhs[1];
	e[2] *= rhs[2];
	return *this;
}

vec3& vec3::operator/=(const vec3& rhs) {
	e[0] /= rhs[0];
	e[1] /= rhs[1];
	e[2] /= rhs[2];
	return *this;
}

// Scalar assignments
vec3& vec3::operator*=(const double t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

vec3& vec3::operator/=(const double t) {
	return *this *= 1 / t;
}

// Static methods
[[nodiscard]] vec3 vec3::random() {
	return vec3(random_double(), random_double(), random_double());
}

[[nodiscard]] vec3 vec3::random(const double min, const double max) {
	return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

//////////////////////////////////////////
// Other vector operator overloads
/////////////////////////////////////////

/*-----Addition/Subtraction operators-----*/
[[nodiscard]] vec3 operator+(const vec3&u, const vec3& v) {
	return vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

[[nodiscard]] vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

/*-----Multiplication operators-----*/
[[nodiscard]] vec3 operator*(const double t, const vec3& v) {
	return vec3(t * v[0], t * v[1], t * v[2]);
}

[[nodiscard]] vec3 operator*(const vec3& v, double t) {
	return t * v;
}

[[nodiscard]] vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

/*-----Division operators-----*/
[[nodiscard]] vec3 operator/(const vec3& v1, const vec3& v2) {
	return vec3(v1[0] / v2[0], v1[1] / v2[1], v1[2] / v2[2]);
}

[[nodiscard]] vec3 operator/(vec3 v, double t) {
	return (1 / t) * v;
}


/*-----Misc. Functions-----*/
[[nodiscard]] vec3 unit_vector(vec3 v) {
	return v / v.length();
}

[[nodiscard]] vec3 cross(const vec3& u, const vec3& v) {
	return vec3(
		u[1] * v[2] - u[2] * v[1],
		u[2] * v[0] - u[0] * v[2],
		u[0] * v[1] - u[1] * v[0]
	);
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

[[nodiscard]] vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n) * n;
}

[[nodiscard]] vec3 refract(const vec3& uv, const vec3& n, const double etai_over_etat) {
	const auto cos_theta = fmin(dot(-uv, n), 1.0);
	const vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	const vec3 r_out_parallel = -sqrt(1.0 - r_out_perp.length_squared()) * n;

	return r_out_parallel + r_out_perp;
}
