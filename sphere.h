#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "ray.h"

#include <cmath>

using namespace std;

class sphere {
    public:
        point3 center;
        double radius;

        sphere() : center(point3()), radius(1.0) {};
        sphere(const point3& c, double r) : center(c), radius(r) {};

        // returns the smaller t value, or -1.0
        double intersect(const ray& r){
            vec3 oc = r.origin() - center;

            auto a = dot(r.direction(),r.direction());
            auto b = 2.0 * dot(oc, r.direction());
            auto c = dot(oc, oc) - radius * radius;
            auto discriminant = b*b - 4*a*c;

            if (discriminant < 0) return -1.0;
            return (-b -sqrt(discriminant)) / (2.0*a);
        }
};

double hit_sphere(const point3& center, double radius, ray& r){
    vec3 oc = r.origin() - center;

    auto a = dot(r.direction(),r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b*b - 4*a*c;
    
    if (discriminant < 0) return -1.0;
    return (-b -sqrt(discriminant)) / (2.0*a);
}

#endif