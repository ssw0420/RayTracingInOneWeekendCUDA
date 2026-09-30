#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"

#include <iostream>

using Color = Vec3;

void WriteColor(std::ostream& out, const Color& pixelColor)
{
	double r = pixelColor.X();
	double g = pixelColor.Y();
	double b = pixelColor.Z();

	// [0, 1)
	static const Interval intensity(0.000, 0.999);

	int rByte = static_cast<int>(256.0 * intensity.Clamp(r));
	int gByte = static_cast<int>(256.0 * intensity.Clamp(g));
	int bByte = static_cast<int>(256.0 * intensity.Clamp(b));

	// Write Color
	out << rByte << ' ' << gByte << ' ' << bByte << '\n';
}

#endif