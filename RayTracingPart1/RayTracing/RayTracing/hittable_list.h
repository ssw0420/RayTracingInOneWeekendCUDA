#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "rtweekend.h"
#include "hittable.h"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class HittableList : public Hittable
{
public:
	HittableList() = default;

	explicit HittableList(const shared_ptr<Hittable>& object)
	{
		Add(object);
	}

	void Clear()
	{
		mObjects.clear();
	}

	void Add(const shared_ptr<Hittable>& object)
	{
		mObjects.push_back(object);
	}

	bool Hit(const Ray& ray, const Interval& rayT, HitRecord& hitRecord) const override
	{
		HitRecord temporaryHitRecord;
		bool bHitAnything = false;
		double closestSoFar = rayT.Max;

		for (const auto& object : mObjects)
		{
			Interval currentRayT(rayT.Min, closestSoFar);
			if (object->Hit(ray, currentRayT, temporaryHitRecord))
			{
				bHitAnything = true;
				closestSoFar = temporaryHitRecord.T;
				hitRecord = temporaryHitRecord;
			}
		}

		return bHitAnything;
	}


private:
	std::vector<shared_ptr<Hittable>> mObjects;
};

#endif