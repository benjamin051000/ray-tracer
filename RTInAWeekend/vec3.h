#pragma once
#include <math.h>
#include <iostream>
using namespace std;

struct vec3 {
	//Constructors
	vec3() {}
	vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }

	//Coord getters
	float x() const { return e[0]; }
	float y() const { return e[1]; }
	float z() const { return e[2]; }

	//General operator overloading
	const vec3& operator+() const { return *this; }
	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	float operator[](int i) const { return e[i]; }
	float& operator[](int i) { return e[i]; } //can this be const?

	//Math-specific operator overloading
	vec3& operator+=(const vec3 &rhs);
	vec3& operator-=(const vec3 &rhs);
	vec3& operator*=(const vec3 &rhs);
	vec3& operator/=(const vec3 &rhs);

	//Multiply by scalar
	vec3& operator*=(const float t);
	vec3& operator/=(const float t);

	float length() const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
	float squared_length() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
	void make_unit_vector();

	//Stores coordinates
	float e[3];
};



//-----Addition operators-----//
inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

//-----Multiplication operators-----//
inline vec3 operator*(const float t, const vec3 &v) {
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3 &v, const float t) {
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

//-----Division operators-----//
inline vec3 operator/(const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

vec3 operator/(const vec3 &v, const float t) {
	return vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

//-----Assignment operators-----//
inline vec3& vec3::operator+=(const vec3& rhs) {
	e[0] += rhs.e[0];
	e[1] += rhs.e[1];
	e[2] += rhs.e[2];
	return *this;
}

inline vec3& vec3::operator-=(const vec3& rhs) {
	e[0] -= rhs.e[0];
	e[1] -= rhs.e[1];
	e[2] -= rhs.e[2];
	return *this;
}

inline vec3& vec3::operator*=(const vec3& rhs) {
	e[0] *= rhs.e[0];
	e[1] *= rhs.e[1];
	e[2] *= rhs.e[2];
	return *this;
}

inline vec3& vec3::operator/=(const vec3 &rhs) {
	e[0] /= rhs.e[0];
	e[1] /= rhs.e[1];
	e[2] /= rhs.e[2];
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
	float k = 1.f / t; //Not sure why tutorial does this. Performance?
	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
	return *this;
}

//Miscellaneous operators
vec3 unit_vector(vec3 v) {
	return v / v.length();
}

inline float dot(const vec3& v1, const vec3& v2) {
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vec3 cross(const vec3& v1, const vec3& v2) {
	return vec3(
		v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
		v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2], //Flipped with - sign in tutorial 
		v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]
	);
}

inline void vec3::make_unit_vector() {
	float k = 1.0 / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
}

//Input/output
inline istream& operator>>(istream& input, vec3& t) {
	input >> t.e[0] >> t.e[1] >> t.e[2];
	return input;
}

inline ostream& operator<<(ostream& output, vec3& t) {
	output << t.e[0] << " " << t.e[1] << " " << t.e[2];
	return output;
}