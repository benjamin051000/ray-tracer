#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const auto infinity = std::numeric_limits<float>::infinity();
const auto PI = 3.1415926535897932385;


// Common functions
double deg_to_rad(double deg);

double clamp(double x, double min, double max);

double random_double();

double random_double(double min, double max);

int random_int(int min, int max);



// Common headers
#include "ray.h"
//#include "vec3.h" // Included in ray.h

