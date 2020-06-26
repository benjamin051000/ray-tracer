#pragma once
#include "common.h"
//#include "aabb.h"

/*Tells the compiler that the hit pointer 
in hit_record points to a class*/
class material;


/*A record of a ray hitting an object.*/
struct hit_record {
	float t = 0;
	point3 p;
	vec3 normal;
	//The object's material.
	shared_ptr<material> material;

	double u = 0, v = 0; // For texture mapping

	bool front_face = false;

	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};


//ABC
struct hittable {
	virtual bool hit(
		const ray& r, 
		float tmin, 
		float tmax, 
		hit_record& rec) const = 0;

	//virtual bool bounding_box(float t0, float t1, aabb& output_box) const = 0;
};