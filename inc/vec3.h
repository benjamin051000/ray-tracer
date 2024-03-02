#pragma once
#include <iostream>


class vec3 {
public:
	//Constructors
	vec3();
	vec3(double e0, double e1, double e2);

	// getters
	double x() const;
	double y() const;
	double z() const;

	//General operator overloading
	vec3 operator-() const;
	double operator[](int i) const;
	double& operator[](int i); // Not const because the reference will allow changes

	//Assignment operator overloading
	vec3& operator+=(const vec3& rhs);
	vec3& operator-=(const vec3& rhs);
	vec3& operator*=(const vec3& rhs);
	vec3& operator/=(const vec3& rhs);

	//Multiply by scalar
	vec3& operator*=(const double t);
	vec3& operator/=(const double t);

	double length() const;
	double length_squared() const;
	//void make_unit_vector();

	bool near_zero() const;

	static vec3 random();

	static vec3 random(double min, double max);

	//Stores coordinates
	double e[3];
};

// Type aliases for vec3
using point3 = vec3;
using color = vec3;

/*-----Addition/Subtraction operators-----*/
vec3 operator+(const vec3& u, const vec3& v);

vec3 operator-(const vec3& u, const vec3& v);

/*-----Multiplication operators-----*/
vec3 operator*(double t, const vec3& v);

vec3 operator*(const vec3& v, double t);

vec3 operator*(const vec3& u, const vec3& v);

/*-----Division operators-----*/
vec3 operator/(const vec3& v1, const vec3& v2);

vec3 operator/(vec3 v, double t);

//Miscellaneous operators
vec3 unit_vector(vec3 v);

double dot(const vec3& u, const vec3& v);

vec3 cross(const vec3& u, const vec3& v);

// cout
std::ostream& operator<<(std::ostream& output, vec3& t);

/*----------Random Vector Generators----------*/
vec3 random_in_unit_disk();

vec3 random_in_unit_sphere();

vec3 random_unit_vector();

vec3 reflect(const vec3& v, const vec3& n);

vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat);

