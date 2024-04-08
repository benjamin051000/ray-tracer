#pragma once
#include <iostream>
#include <cmath>


struct vec3 {
	//Constructors
	vec3();
	vec3(const double e0, const double e1, const double e2);

	// getters
	constexpr double x() const { return e[0]; }
	constexpr double y() const { return e[1]; }
	constexpr double z() const { return e[2]; }


	//General operator overloading
	[[nodiscard]] vec3 operator-() const; 

	[[nodiscard]] constexpr double operator[](int i) const { 
		return e[i];
	}

	[[nodiscard]] constexpr double& operator[](int i) {
		return e[i];
	}

	//Assignment operator overloading
	vec3& operator+=(const vec3& rhs);
	vec3& operator-=(const vec3& rhs);
	vec3& operator*=(const vec3& rhs);
	vec3& operator/=(const vec3& rhs);

	//Multiply by scalar
	vec3& operator*=(const double t);
	vec3& operator/=(const double t);


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
	double e[3];
};

// Type aliases for vec3
using point3 = vec3;
using color = vec3;

/*-----Addition/Subtraction operators-----*/
[[nodiscard]] vec3 operator+(const vec3& u, const vec3& v);

[[nodiscard]] vec3 operator-(const vec3& u, const vec3& v);

/*-----Multiplication operators-----*/
[[nodiscard]] vec3 operator*(const double t, const vec3& v);

[[nodiscard]] vec3 operator*(const vec3& v, double t);

[[nodiscard]] vec3 operator*(const vec3& u, const vec3& v);

/*-----Division operators-----*/
[[nodiscard]] vec3 operator/(const vec3& v1, const vec3& v2);

[[nodiscard]] vec3 operator/(vec3 v, double t);

//Miscellaneous operators
[[nodiscard]] vec3 unit_vector(vec3 v);

[[nodiscard]] constexpr double dot(const vec3& u, const vec3& v) {
	return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

[[nodiscard]] vec3 cross(const vec3& u, const vec3& v);

// cout
std::ostream& operator<<(std::ostream& output, vec3& t);

/*----------Random Vector Generators----------*/
[[nodiscard]] vec3 random_in_unit_disk();

[[nodiscard]] vec3 random_in_unit_sphere();

[[nodiscard]] vec3 random_unit_vector();

[[nodiscard]] vec3 reflect(const vec3& v, const vec3& n);

[[nodiscard]] vec3 refract(const vec3& uv, const vec3& n, const double etai_over_etat);

