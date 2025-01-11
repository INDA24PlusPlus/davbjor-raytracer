#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "ray.h"

#include <iostream>

using namespace std;

void write_color(std::ostream &out, color pixel_color){
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) <<  ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

color getColor(const ray& r){
    vec3 unit_v = normalize(r.direction());
    auto t = 0.5*(unit_v.y() + 1.0);
    return (1.0-t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

#endif