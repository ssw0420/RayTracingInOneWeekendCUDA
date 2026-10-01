#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

#include <memory>

class Sphere : public Hittable
{
public:
	Sphere(const Point3& center, double radius, const std::shared_ptr<Material>& material)
		:mCenter(center), mRadius(std::fmax(0.0, radius)), mMaterial(material)
	{
	}

	bool Hit(const Ray& ray, const Interval& rayT, HitRecord& hitRecord) const override
	{		
		Vec3 originToCenter = mCenter - ray.Origin();

		double a = ray.Direction().LengthSquared();
		double h = Dot(ray.Direction(), originToCenter);
		double c = originToCenter.LengthSquared() - mRadius * mRadius;
		double discriminant = h * h - a * c;

		if (discriminant < 0.0)
		{
			return false;
		}

		double sqrtDiscriminant = std::sqrt(discriminant);

		double result = (h - sqrtDiscriminant) / a;
		if (!rayT.Surrounds(result))
		{
			result = (h + sqrtDiscriminant) / a;

			if (!rayT.Surrounds(result))
			{
				return false;
			}
		}

		hitRecord.T = result;
		hitRecord.P = ray.At(hitRecord.T);

		Vec3 outwardNormal = (hitRecord.P - mCenter) / mRadius;
		hitRecord.SetFaceNormal(ray, outwardNormal);

		hitRecord.material = mMaterial;
		return true;
	}


private:
	Point3 mCenter;
	double mRadius;
	std::shared_ptr<Material> mMaterial;
};

#endif