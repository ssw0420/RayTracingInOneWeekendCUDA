#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class Camera
{
public:
	void Render(const Hittable& world)
	{
		Initialize();

		std::cout << "P3\n" << mImageWidth << " " << mImageHeight << "\n255\n";

		for (int scanlineIndex = 0; scanlineIndex < mImageHeight; ++scanlineIndex)
		{
			std::clog << "\rScanlines remaining: " << (mImageHeight - scanlineIndex) << ' ' << std::flush;

			for (int pixelIndex = 0; pixelIndex < mImageWidth; ++pixelIndex)
			{
				Point3 currentPixelCenter = mFirstPixelCenter + (pixelIndex * mPixelDeltaU) + (scanlineIndex * mPixelDeltaV);
				Vec3 rayDirection = currentPixelCenter - mCenter;
				Ray ray(mCenter, rayDirection);

				Color pixelColor = RayColor(ray, world);
				WriteColor(std::cout, pixelColor);
			}
		}

		std::clog << "\rDone.                   \n";
	}

private:
	void Initialize()
	{
		mImageHeight = static_cast<int>(mImageWidth / mAspectRatio);
		mImageHeight = (mImageHeight < 1) ? 1 : mImageHeight;

		mCenter = Point3(0.0, 0.0, 0.0);

		double focalLength = 1.0;
		double viewportHeight = 2.0;
		double viewportWidth = viewportHeight * (double(mImageWidth) / mImageHeight);

		Vec3 viewportU = Vec3(viewportWidth, 0.0, 0.0);
		Vec3 viewportV = Vec3(0.0, -viewportHeight, 0.0);

		// ÇÈ¼¿ °£ ¼öÆò ¹× ¼öÁ÷ µ¨Å¸ º¤ÅÍ
		mPixelDeltaU = viewportU / mImageWidth;
		mPixelDeltaV = viewportV / mImageHeight;

		Point3 viewportUpperLeft = mCenter - Vec3(0.0, 0.0, focalLength) - viewportU / 2.0 - viewportV / 2.0;
		mFirstPixelCenter = viewportUpperLeft + 0.5 * (mPixelDeltaU + mPixelDeltaV);
	}

	Color RayColor(const Ray& ray, const Hittable& world)
	{
		HitRecord hitRecord;
		if (world.Hit(ray, Interval(0.0, Infinity), hitRecord))
		{
			return (0.5 * (hitRecord.Normal + Color(1.0, 1.0, 1.0)));
		}

		Vec3 unitDirection = UnitVector(ray.Direction());
		double a = 0.5 * (unitDirection.Y() + 1.0);

		return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
	}

public:
	double mAspectRatio = 1.0; // Ratio of image width over height
	int mImageWidth = 100; // width pixel count

private:
	int mImageHeight = 0;
	Point3 mCenter;
	Point3 mFirstPixelCenter;
	Vec3 mPixelDeltaU;
	Vec3 mPixelDeltaV;
};

#endif