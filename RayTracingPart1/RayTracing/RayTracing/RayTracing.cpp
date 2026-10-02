#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>

int main()
{
    //// World
    //HittableList world;

    //auto materialGround = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    //auto materialCenter = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    //auto materialLeft = std::make_shared<Dielectric>(1.50);
    //auto materialBubble = std::make_shared<Dielectric>(1.0 / 1.50);
    //auto materialRight = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

    //world.Add(std::make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, materialGround));
    //world.Add(std::make_shared<Sphere>(Point3(0.0, 0.0, -1.2), 0.5, materialCenter));

    //world.Add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, materialLeft));
    //world.Add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.4, materialBubble));

    //world.Add(std::make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, materialRight));

    ////auto R = std::cos(Pi / 4);

    ////auto materialLeft = make_shared<Lambertian>(Color(0, 0, 1));
    ////auto materialRight = make_shared<Lambertian>(Color(1, 0, 0));

    ////world.Add(make_shared<Sphere>(Point3(-R, 0, -1), R, materialLeft));
    ////world.Add(make_shared<Sphere>(Point3(R, 0, -1), R, materialRight));

    //Camera camera;
    //camera.mAspectRatio = 16.0 / 9.0;
    //camera.mImageWidth = 400;
    //camera.mSamplesPerPixel = 100;
    //camera.mMaxDepth = 50;

    //camera.vfov = 20;
    //camera.lookfrom = Point3(-2, 2, 1);
    //camera.lookat = Point3(0, 0, -1);
    //camera.vup = Vec3(0, 1, 0);

    //camera.defocus_angle = 10.0;
    //camera.focus_distance = 3.4;

    HittableList world;

    auto groundMaterial = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.Add(std::make_shared<Sphere>(Point3(0.0, -1000.0, 0.0), 1000.0, groundMaterial));

    for (int gridX = -11; gridX < 11; ++gridX)
    {
        for (int gridZ = -11; gridZ < 11; ++gridZ)
        {
            const double materialSelector = RandomDouble();

            const Point3 center(gridX + 0.9 * RandomDouble(), 0.2, gridZ + 0.9 * RandomDouble());

            if ((center - Point3(4.0, 0.2, 0.0)).Length() > 0.9)
            {
                std::shared_ptr<Material> sphereMaterial;

                if (materialSelector < 0.8)
                {
                    const Color albedo = Color::Random() * Color::Random();
                    sphereMaterial = std::make_shared<Lambertian>(albedo);
                }
                else if (materialSelector < 0.95)
                {
                    const Color albedo = Color::Random(0.5, 1.0);
                    const double fuzz = RandomDouble(0.0, 0.5);
                    sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
                }
                else
                {
                    sphereMaterial = std::make_shared<Dielectric>(1.5);
                }

                world.Add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.Add(std::make_shared<Sphere>(Point3(0.0, 1.0, 0.0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.Add(std::make_shared<Sphere>(Point3(-4.0, 1.0, 0.0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.Add(std::make_shared<Sphere>(Point3(4.0, 1.0, 0.0), 1.0, material3));

    Camera camera;

    camera.mAspectRatio = 16.0 / 9.0;
    camera.mImageWidth = 1920;
    camera.mSamplesPerPixel = 500;
    camera.mMaxDepth = 50;

    camera.vfov = 20.0;
    camera.lookfrom = Point3(13.0, 2.0, 3.0);
    camera.lookat = Point3(0.0, 0.0, 0.0);
    camera.vup = Vec3(0.0, 1.0, 0.0);

    camera.defocus_angle = 0.6;
    camera.focus_distance = 10.0;

    camera.Render(world);
}