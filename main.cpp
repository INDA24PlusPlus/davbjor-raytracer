#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "sphere.h"
#include "hittable_list.h"
#include "color.h"
#include "camera.h"
#include "material.h"
#include "utils.h"

#include <iostream>
#include <fstream>
#include <vector>

#define infinity std::numeric_limits<double>::infinity()

int main(){
    // World
    // Materials
    auto material_left = make_shared<metal>(color(0.1, 0.7, 0.2), 0);
    auto material_right = make_shared<lambertian>(color(0.2, 0.1, 0.7));
    //auto material_right = make_shared<dielectic>(1.05);


    auto material_ground = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    auto material_center = make_shared<lambertian>(color(0.7, 0.2, 0.1));

    // Creating objects
    /*sphere s1(vec3(-1, 0, -1), 0.5, color(0.4, 0.9, 0.3), true);
    sphere s2(vec3(0, 0, -1), 0.5, material_center);
    sphere s3(vec3(1, 0, -1), 0.5, color(0.9, 0.3, 0.4), true);
    sphere s4(vec3(0, -100.5, -1), 100, material_ground);*/

    hittable_list world;
    world.add(make_shared<sphere>(vec3(-2, 0.5, -2), 1, material_left));
    world.add(make_shared<sphere>(vec3(0, 0.5, -2), 1, material_center));
    world.add(make_shared<sphere>(vec3(-0.55, 0, -1), 0.25, material_right));
    
    world.add(make_shared<sphere>(vec3(0, -100.5, -1), 100, material_ground));


    camera cam;
    cam.screen_width = 5200;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.max_depth = 6;

    cam.render(world);

    return 0;
}
