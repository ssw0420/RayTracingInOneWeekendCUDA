#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>

int main()
{
    // World
    HittableList world;
    world.Add(make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5));
    world.Add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0));

    Camera camera;
    camera.mAspectRatio = 16.0 / 9.0;
    camera.mImageWidth = 400;
    camera.mSamplesPerPixel = 100;

    camera.Render(world);
}