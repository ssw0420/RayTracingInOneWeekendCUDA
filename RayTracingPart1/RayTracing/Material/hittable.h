#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

class Material;

class HitRecord
{
public:
	void SetFaceNormal(const Ray& ray, const Vec3& outwardNormal)
	{
		bFrontFace = Dot(ray.Direction(), outwardNormal) < 0;
		Normal = bFrontFace ? outwardNormal : -outwardNormal;
	}

public:
	Point3 P;
	Vec3 Normal;
	double T;
	bool bFrontFace;
	std::shared_ptr<Material> material;
};

class Hittable
{
public:
	virtual ~Hittable() = default;

	virtual bool Hit(const Ray& ray, const Interval& rayT, HitRecord& hitRecord) const = 0;
};

#endif