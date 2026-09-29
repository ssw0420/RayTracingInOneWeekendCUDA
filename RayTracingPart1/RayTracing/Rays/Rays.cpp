#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>

bool HitSphere(const Point3& center, double radius, const Ray& r)
{
    Vec3 originToCenter = center - r.Origin();
    double a = Dot(r.Direction(), r.Direction());
    double b = -2.0 * Dot(r.Direction(), originToCenter);
    double c = Dot(originToCenter, originToCenter) - radius * radius;
    double discriminant = b * b - 4 * a * c;
    return (discriminant >= 0);
}

Color RayColor(const Ray& r)
{
    if (HitSphere(Point3(0, 0, -1), 0.5, r))
    {
        return Color(1, 0, 0);
    }

    Vec3 unitDirection = UnitVector(r.Direction());
    double a = 0.5 * (unitDirection.Y() + 1.0);

    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

int main()
{
    double aspectRatio = 16.0 / 9.0;
    int imageWidth = 400;

    // 최소 1의 높이를 보장
    int imageHeight = int(imageWidth / aspectRatio);
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;

    // Camera
    double focalLength = 1.0;
    double viewportHeight = 2.0;
    double viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);
    Point3 cameraCenter = Point3(0, 0, 0);

    Vec3 viewportU = Vec3(viewportWidth, 0, 0);
    Vec3 viewportV = Vec3(0, -viewportHeight, 0);

    // 픽셀 간 수평 및 수직 델타 벡터
    Vec3 pixelDeltaU = viewportU / imageWidth;
    Vec3 pixelDeltaV = viewportV / imageHeight;

    Point3 viewportUpperLeft = cameraCenter - Vec3(0, 0, focalLength) - viewportU / 2 - viewportV / 2;
    Point3 firstPixelCenter = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
    
    std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    for (int j = 0; j < imageHeight; ++j)
    {
        std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;

        for (int i = 0; i < imageWidth; ++i)
        {
            Point3 currentPixelCenter = firstPixelCenter + (i * pixelDeltaU) + (j * pixelDeltaV);
            Vec3 rayDirection = currentPixelCenter - cameraCenter;
            Ray r(cameraCenter, rayDirection);

            Color pixelColor = RayColor(r);
            WriteColor(std::cout, pixelColor);
        }
    }

    std::clog << "\rDone.                   \n";
}