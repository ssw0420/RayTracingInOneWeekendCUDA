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
				Color pixelColor(0.0, 0.0, 0.0);

				for (int sampleIndex = 0; sampleIndex < mSamplesPerPixel; ++sampleIndex)
				{
					Ray ray = GetRay(pixelIndex, scanlineIndex);
					pixelColor += RayColor(ray, mMaxDepth, world);
				}

				WriteColor(std::cout, mPixelSamplesScale * pixelColor);
			}
		}

		std::clog << "\rDone.                   \n";
	}

private:
	void Initialize()
	{
		mImageHeight = static_cast<int>(mImageWidth / mAspectRatio);
		mImageHeight = (mImageHeight < 1) ? 1 : mImageHeight;

		mPixelSamplesScale = 1.0 / static_cast<double>(mSamplesPerPixel);

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

	Color RayColor(const Ray& ray, int depth, const Hittable& world)
	{
		if (depth <= 0)
		{
			return Color(0.0, 0.0, 0.0);
		}

		HitRecord hitRecord;
		if (world.Hit(ray, Interval(0.001, Infinity), hitRecord))
		{
			Vec3 direction = hitRecord.Normal + RandomOnHemisphere(hitRecord.Normal);
			return 0.9 * RayColor(Ray(hitRecord.P, direction), depth - 1, world);
		}

		Vec3 unitDirection = UnitVector(ray.Direction());
		double a = 0.5 * (unitDirection.Y() + 1.0);

		return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0); // background
	}

	Ray GetRay(int pixelIndex, int scanlineIndex) const
	{
		Vec3 offset = SampleSquare();

		Vec3 pixelSample = mFirstPixelCenter + ((pixelIndex + offset.X()) * mPixelDeltaU)
			+ ((scanlineIndex + offset.Y()) * mPixelDeltaV);

		Point3 rayOrigin = mCenter;
		Vec3 rayDirection = pixelSample - rayOrigin;

		return Ray(rayOrigin, rayDirection);
	}

	Vec3 SampleSquare() const
	{
		return Vec3(RandomDouble() - 0.5, RandomDouble() - 0.5, 0.0);
	}


public:
	double mAspectRatio = 1.0; // Ratio of image width over height
	int mImageWidth = 100; // width pixel count

	int mSamplesPerPixel = 10;

	int mMaxDepth = 10;

private:
	int mImageHeight = 0;
	Point3 mCenter;
	Point3 mFirstPixelCenter;
	Vec3 mPixelDeltaU;
	Vec3 mPixelDeltaV;

	double mPixelSamplesScale = 1.0;
};

#endif