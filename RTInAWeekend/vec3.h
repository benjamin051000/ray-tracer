#pragma once
#include <math.h>
#include <stdlib.h> //What is this for?
#include <iostream>
using namespace std;

class vec3 {
public:
	//Constructors
	vec3() {}
	vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }

	//Coord getters
	float x() const { return e[0]; }
	float y() const { return e[1]; }
	float z() const { return e[3]; }

	//General operator overloading
	const vec3& operator+() const { return *this; }
	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	float operator[](int i) const { return e[i]; }
	float& operator[](int i) { return e[i]; } //can this be const?

	//Math-specific operator overloading
	vec3& operator+=(const vec3 & rhs);
	vec3& operator-=(const vec3 & rhs);
	vec3& operator*=(const vec3 & rhs);
	vec3& operator/=(const vec3 & rhs);

	//Multiply by scalar
	vec3& operator*=(const float t);
	vec3& operator/=(const float t);

	float length() const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
	float squared_length() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
	void make_unit_vector();

	//Stores coordinates
	float e[3];
};

inline istream& operator>>(istream & input, vec3 & t) {
	input >> t.e[0] >> t.e[1] >> t.e[2];
	return input;
}

inline ostream& operator<<(ostream & output, vec3 & t) {
	output << t.e[0] << " " << t.e[1] << " " << t.e[2];
	return output;
}

inline void vec3::make_unit_vector() {
	float k = 1.f / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
}

inline vec3 operator+(const vec3 & v1, const vec3 & v2) {
	return vec3();
}