#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "rtweekend.h"

class Material
{
public:
	virtual ~Material() = default;

	virtual bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, Color& attenuation, Ray& scattered) const
	{
		return false;
	}
};

class Lambertian : public Material
{
public:
	explicit Lambertian(const Color& albedo)
		: mAlbedo(albedo)
	{
	}

	bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, Color& attenuation, Ray& scattered) const override
	{
		Vec3 scatterDirection = hitRecord.Normal + RandomUnitVector();

		if (scatterDirection.NearZero())
		{
			scatterDirection = hitRecord.Normal;
		}

		scattered = Ray(hitRecord.P, scatterDirection);
		attenuation = mAlbedo;

		return true;
	}

private:
	Color mAlbedo;
};

class Metal : public Material
{
public:
	explicit Metal(const Color& albedo, double fuzz)
		: mAlbedo(albedo), mFuzz(fuzz < 1.0 ? fuzz : 1.0)
	{
	}

	bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, Color& attenuation, Ray& scattered) const override
	{
		Vec3 reflected = Reflect(rayIn.Direction(), hitRecord.Normal);

		// fuzz 구의 중심까지 벡터 + 중심에서 표면까지 벡터
		reflected = UnitVector(reflected) + (mFuzz * RandomUnitVector());

		scattered = Ray(hitRecord.P, reflected);
		attenuation = mAlbedo;

		return (Dot(scattered.Direction(), hitRecord.Normal) > 0);
	}

private:
	Color mAlbedo;
	double mFuzz;
};

class Dielectric : public Material
{
public:
	explicit Dielectric(double refractionIndex)
		: mRefractionIndex(refractionIndex)
	{
	}

	bool Scatter(const Ray& rayIn, const HitRecord& hitRecord, Color& attenuation, Ray& scattered) const override
	{
		attenuation = Color(1.0, 1.0, 1.0);

		const double refractionRatio = hitRecord.bFrontFace ? (1.0 / mRefractionIndex) : mRefractionIndex;

		const Vec3 unitDirection = UnitVector(rayIn.Direction());

		const double cosTheta = std::fmin(Dot(-unitDirection, hitRecord.Normal), 1.0);
		const double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

		const bool cannotRefract = refractionRatio * sinTheta > 1.0;

		Vec3 direction;

		if (cannotRefract || Reflectance(cosTheta, refractionRatio) > RandomDouble())
		{
			direction = Reflect(unitDirection, hitRecord.Normal);
		}
		else
		{
			direction = Refract(unitDirection, hitRecord.Normal, refractionRatio, cosTheta);
		}

		scattered = Ray(hitRecord.P, direction);

		return true;
	}
private:
	static double Reflectance(double cos, double refractionIndex)
	{
		// Schlick Approximation
		auto r0 = (1.0 - refractionIndex) / (1.0 + refractionIndex);
		r0 = r0 * r0;

		return r0 + (1.0 - r0) * std::pow((1.0 - cos), 5);
	}

private:
	double mRefractionIndex = 1.0;
};

#endif