#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class Sphere : public Hittable
{
public:
	Sphere(const Point3& center, double radius)
		:mCenter(center), mRadius(std::fmax(0.0, radius))
	{
	}

	bool Hit(const Ray& ray, const Interval& rayT, HitRecord& hitRecord) const override
	{
		//Vec3 originToCenter = center - r.Origin();
		//double a = Dot(r.Direction(), r.Direction());
		//double b = -2.0 * Dot(r.Direction(), originToCenter);
		//double c = Dot(originToCenter, originToCenter) - radius * radius;
		//double discriminant = b * b - 4 * a * c;
		//
		//// 기존에는 판별식을 통해 근의 개수 정보만 필요했지만, 실제 충돌점을 구하기 위해서 근의공식 전체를 사용
		//if (discriminant < 0.0)
		//{
		//    return -1.0;
		//}

		//return (-b - std::sqrt(discriminant)) / (2.0 * a); // - 를 사용하였으므로 항상 작은 값인 가까운 충돌 지점이 나옴
		
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

		return true;
	}


private:
	Point3 mCenter;
	double mRadius;
};

#endif