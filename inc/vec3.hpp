#pragma once
#include <iostream>
#include <cmath>


struct vec3 {
	//Constructors
	constexpr vec3() = default;
	constexpr vec3(const double e0, const double e1, const double e2) : e{ e0, e1, e2 } {}

	// getters
	[[nodiscard]] constexpr double x() const { return e[0]; }
	[[nodiscard]] constexpr double y() const { return e[1]; }
	[[nodiscard]] constexpr double z() const { return e[2]; }


	//General operator overloading
	[[nodiscard]] constexpr vec3 operator-() const { 
		return vec3(-e[0], -e[1], -e[2]); 
	}

	[[nodiscard]] constexpr double operator[](int i) const { 
		return e[i];
	}

	[[nodiscard]] constexpr double& operator[](int i) {
		return e[i];
	}

	//Assignment operator overloading
	/*-----Assignment operators-----*/
	constexpr vec3& operator+=(const vec3& rhs) {
		e[0] += rhs[0];
		e[1] += rhs[1];
		e[2] += rhs[2];
		return *this;
	}

	constexpr vec3& operator-=(const vec3& rhs) {
		e[0] -= rhs[0];
		e[1] -= rhs[1];
		e[2] -= rhs[2];
		return *this;
	}

	constexpr vec3& operator*=(const vec3& rhs) {
		e[0] *= rhs[0];
		e[1] *= rhs[1];
		e[2] *= rhs[2];
		return *this;
	}

	constexpr vec3& operator/=(const vec3& rhs) {
		e[0] /= rhs[0];
		e[1] /= rhs[1];
		e[2] /= rhs[2];
		return *this;
	}

	// Scalar assignments
	constexpr vec3& operator*=(const double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	constexpr vec3& operator/=(const double t) {
		return *this *= 1 / t;
	}

	[[nodiscard]] constexpr double length() const {
		return sqrt(length_squared());
	}

	[[nodiscard]] constexpr double length_squared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	//void make_unit_vector();

	[[nodiscard]] constexpr bool near_zero() const {
		constexpr auto s = 1e-8;
		return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
	}

	static vec3 random();

	static vec3 random(const double min, const double max);

	//Stores coordinates
	double e[3]{0, 0, 0};
};

// Type aliases for vec3
using point3 = vec3;
using color = vec3;

/*-----Addition/Subtraction operators-----*/
[[nodiscard]] constexpr vec3 operator+(const vec3&u, const vec3& v) {
	return vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

[[nodiscard]] constexpr vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

/*-----Multiplication operators-----*/
[[nodiscard]] constexpr vec3 operator*(const double t, const vec3& v) {
	return vec3(t * v[0], t * v[1], t * v[2]);
}

[[nodiscard]] constexpr vec3 operator*(const vec3& v, double t) {
	return t * v;
}

[[nodiscard]] constexpr vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

/*-----Division operators-----*/
[[nodiscard]] constexpr vec3 operator/(const vec3& v1, const vec3& v2) {
	return vec3(v1[0] / v2[0], v1[1] / v2[1], v1[2] / v2[2]);
}

[[nodiscard]] constexpr vec3 operator/(vec3 v, double t) {
	return (1 / t) * v;
}

/*-----Misc. Functions-----*/
[[nodiscard]] constexpr vec3 unit_vector(vec3 v) {
	return v / v.length();
}

[[nodiscard]] constexpr double dot(const vec3& u, const vec3& v) {
	return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

[[nodiscard]] constexpr vec3 cross(const vec3& u, const vec3& v) {
	return vec3(
		u[1] * v[2] - u[2] * v[1],
		u[2] * v[0] - u[0] * v[2],
		u[0] * v[1] - u[1] * v[0]
	);
}

std::ostream& operator<<(std::ostream& output, vec3& t);

/*----------Random Vector Generators----------*/
[[nodiscard]] vec3 random_in_unit_disk();

[[nodiscard]] vec3 random_in_unit_sphere();

[[nodiscard]] vec3 random_unit_vector();

[[nodiscard]] constexpr vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n) * n;
}

[[nodiscard]] constexpr vec3 refract(const vec3& uv, const vec3& n, const double etai_over_etat) {
	const auto cos_theta = fmin(dot(-uv, n), 1.0);
	const vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	const vec3 r_out_parallel = -sqrt(1.0 - r_out_perp.length_squared()) * n;

	return r_out_parallel + r_out_perp;
}
