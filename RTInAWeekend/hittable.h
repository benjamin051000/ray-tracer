#pragma once
#include "ray.h"

/*Tells the compiler that the hit pointer 
in hit_record points to a class*/
class material;

/*A record of a ray hitting an object.*/
struct hit_record {
	float t;
	vec3 p, normal;
	//The object's material.
	material* material;
};


//ABC
struct hittable {
	virtual bool hit(
		const ray& r, 
		float tmin, 
		float tmax, 
		hit_record& rec) const = 0;
};