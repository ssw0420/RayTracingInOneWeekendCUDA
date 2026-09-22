#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray
{
public:
	Ray()
	{
	}

	Ray(const Point3& origin, const Vector3& direction)
		: mOrigin(origin), mDirection(direction)
	{
	}

	const Point3& Origin() const
	{
		return mOrigin;
	}
	
	const Vector3& Direction() const
	{
		return mDirection;
	}

	Point3 At(double t) const
	{
		return mOrigin + t * mDirection;
	}

private:
	Point3 mOrigin;
	Vector3 mDirection;
};

#endif