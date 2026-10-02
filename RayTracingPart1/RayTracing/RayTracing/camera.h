#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "Material.h"

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

		mCenter = lookfrom;

		double focalLength = (lookfrom - lookat).Length();
		double theta = DegreesToRadians(vfov);
		double h = std::tan(theta / 2);
		double viewportHeight = 2.0 * h * focus_distance;
		double viewportWidth = viewportHeight * (double(mImageWidth) / mImageHeight);

		w = UnitVector(lookfrom - lookat);
		u = UnitVector(Cross(vup, w));
		v = Cross(w, u);

		Vec3 viewportU = viewportWidth * u; // 뷰포트 가로 끝에서 끝 (오른쪽으로)
		Vec3 viewportV = viewportHeight * -v; // 뷰포트 세로 끝에서 끝 (아래로)

		// 픽셀 간 수평 및 수직 델타 벡터
		mPixelDeltaU = viewportU / mImageWidth;
		mPixelDeltaV = viewportV / mImageHeight;

		Point3 viewportUpperLeft = mCenter - (focus_distance * w) - viewportU / 2.0 - viewportV / 2.0;
		mFirstPixelCenter = viewportUpperLeft + 0.5 * (mPixelDeltaU + mPixelDeltaV);

		double defocusRadius = focus_distance * std::tan(DegreesToRadians(defocus_angle * 0.5));

		mDefocusDiskU = u * defocusRadius;
		mDefocusDiskV = v * defocusRadius;
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
			Ray scattered;
			Color attenuation;

			if (hitRecord.material->Scatter(ray, hitRecord, attenuation, scattered))
			{
				return attenuation * RayColor(scattered, depth - 1, world);
			}
			return Color(0.0, 0.0, 0.0);
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

		Point3 rayOrigin = (defocus_angle <= 0.0) ? mCenter : DefocusDiskSample();
		Vec3 rayDirection = pixelSample - rayOrigin;

		return Ray(rayOrigin, rayDirection);
	}

	Vec3 SampleSquare() const
	{
		return Vec3(RandomDouble() - 0.5, RandomDouble() - 0.5, 0.0);
	}

	Point3 DefocusDiskSample() const
	{
		// 1.0 크기를 실제 렌즈에 맞춰서 크기 조절함
		const Vec3 point = RandomInUnitDisk();
		return mCenter + (point.X() * mDefocusDiskU) + (point.Y() * mDefocusDiskV);
	}


public:
	double mAspectRatio = 1.0; // Ratio of image width over height
	int mImageWidth = 100; // width pixel count

	int mSamplesPerPixel = 10;

	int mMaxDepth = 10;

	double vfov = 90; // 수직 시야각
	Point3 lookfrom = Point3(0, 0, 0);
	Point3 lookat = Point3(0, 0, -1);
	Vec3 vup = Vec3(0, 1, 0); // 카메라의 위쪽 방향

	double defocus_angle = 0;
	double focus_distance = 10;

private:
	int mImageHeight = 0;
	Point3 mCenter;
	Point3 mFirstPixelCenter;
	Vec3 mPixelDeltaU;
	Vec3 mPixelDeltaV;

	Vec3 u, v, w; // 카메라 프레임 기저 벡터

	double mPixelSamplesScale = 1.0;

	Vec3 mDefocusDiskU;
	Vec3 mDefocusDiskV;
};

#endif