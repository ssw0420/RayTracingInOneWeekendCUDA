#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

// Constants
constexpr double Infinity = std::numeric_limits<double>::infinity();
constexpr double Pi = 3.141592653589793285;

// Utility Functions
inline double DegreesToRadians(double degrees)
{
	return degrees * Pi / 180.0;
}

inline double RandomDouble()
{
	// [0, 1)
	// return std::rand() / (RAND_MAX + 1.0);

	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double RandomDouble(double minimum, double maximum)
{
	// [minimum, maximum)
	return minimum + (maximum - minimum) * RandomDouble();
}

// Common Headers

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif