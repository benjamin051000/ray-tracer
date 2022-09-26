#include "common.h"


// Common functions
double deg_to_rad(double deg) {
	return deg * PI / 180;
}

double clamp(double x, double min, double max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

double random_double() {
	return rand() / (RAND_MAX + 1.0);
}

double random_double(double min, double max) {
	return min + (max - min) * random_double();
}

int random_int(int min, int max) {
	return static_cast<int>(random_double(min, max + 1));
}
