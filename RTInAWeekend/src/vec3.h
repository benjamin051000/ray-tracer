#pragma once
#include <math.h>
#include <iostream>


struct vec3 {
	//Constructors
	vec3() : e{ 0,0,0 } {}
	vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }

	//Coord getters
	float x() const { return e[0]; }
	float y() const { return e[1]; }
	float z() const { return e[2]; }

	//General operator overloading
	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	inline float operator[](int i) const { return e[i]; }
	inline float& operator[](int i) { return e[i]; } // Not const because the reference will allow changes

	//Assignment operator overloading
	vec3& operator+=(const vec3& rhs);
	vec3& operator-=(const vec3& rhs);
	vec3& operator*=(const vec3& rhs);
	vec3& operator/=(const vec3& rhs);

	//Multiply by scalar
	vec3& operator*=(const float t);
	vec3& operator/=(const float t);

	float length() const { return sqrt(length_squared()); }
	float length_squared() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
	void make_unit_vector();

	inline static vec3 random() {
		return vec3(random_float(), random_float(), random_float());
	}

	inline static vec3 random(double min, double max) {
		return vec3(random_float(min, max), random_float(min, max), random_float(min, max));
	}

	//Stores coordinates
	float e[3];
};

// Type aliases for vec3
using point3 = vec3;
using color = vec3;

//-----Addition operators-----//
inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

//-----Multiplication operators-----//
inline vec3 operator*(const float t, const vec3& v) {
	return vec3(t * v[0], t * v[1], t * v[2]);
}

inline vec3 operator*(const vec3& v, const float t) {
	return t * v;
}

inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

//-----Division operators-----//
inline vec3 operator/(const vec3& v1, const vec3& v2) {
	return vec3(v1[0] / v2[0], v1[1] / v2[1], v1[2] / v2[2]);
}

vec3 operator/(const vec3& v, const float t) {
	return vec3(v[0] / t, v[1] / t, v[2] / t);
}

//-----Assignment operators-----//
inline vec3& vec3::operator+=(const vec3& rhs) {
	e[0] += rhs[0];
	e[1] += rhs[1];
	e[2] += rhs[2];
	return *this;
}

inline vec3& vec3::operator-=(const vec3& rhs) {
	e[0] -= rhs[0];
	e[1] -= rhs[1];
	e[2] -= rhs[2];
	return *this;
}

inline vec3& vec3::operator*=(const vec3& rhs) {
	e[0] *= rhs[0];
	e[1] *= rhs[1];
	e[2] *= rhs[2];
	return *this;
}

inline vec3& vec3::operator/=(const vec3& rhs) {
	e[0] /= rhs[0];
	e[1] /= rhs[1];
	e[2] /= rhs[2];
	return *this;
}

//Scalar assignments
inline vec3& vec3::operator*=(const float t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline vec3& vec3::operator/=(const float t) {
	return *this *= 1 / t;
}

//Miscellaneous operators
vec3 unit_vector(vec3 v) {
	return v / v.length();
}

inline float dot(const vec3& u, const vec3& v) {
	return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(
		u[1] * v[2] - u[2] * v[1],
		u[2] * v[0] - u[0] * v[2],
		u[0] * v[1] - u[1] * v[0]
	);
}

inline void vec3::make_unit_vector() {
	float k = 1.f / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
}

//Output
inline std::ostream& operator<<(std::ostream& output, vec3& t) {
	output << t[0] << " " << t[1] << " " << t[2];
	return output;
}

// For camera
vec3 random_in_unit_disk() {
	vec3 p;
	do
		p = 2 * vec3(random_float(-1, 1), random_float(-1, 1), 0);
	while (p.length_squared() >= 1);
	
	return p;
}