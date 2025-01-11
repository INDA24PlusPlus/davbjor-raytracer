#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "material.h"

#include <cmath>

using namespace std;

class sphere : public hittable {
    public:
        point3 center;
        double radius;
        shared_ptr<material> mat;

        sphere() : center(point3()), radius(1.0) {};
        sphere(const point3& c, double r, shared_ptr<material> m) : center(c), radius(r), mat(m) {};

        // returns the smaller t value, or -1.0
        double intersect(const ray& r) const;

        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
};

double sphere::intersect(const ray& r) const {
    vec3 oc = r.origin() - center;

    auto a = dot(r.direction(),r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b*b - 4*a*c;

    if (discriminant < 0) return -1.0;
    return (-b -sqrt(discriminant)) / (2.0*a);
}

bool sphere::hit(const ray& r, interval ray_t, hit_record& rec) const {
    vec3 oc = r.origin() - center;

    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b*half_b - a*c;
    
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Returns the nearest root that lies in the acceptable range
    auto root = (-half_b - sqrtd) / a;
    if(!ray_t.surrounds(root)){
        return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p - center) / radius;

    /*
    vec3 direction = reflect(r.direction(), rec.normal);
    vec3 random_direction = rec.normal + random_in_unit_sphere();
    vec3 semi_diffuse_direction = unit_vector(0.8 * direction + 0.2 * random_direction);
    */
   
    rec.mat = mat;
    
    return true;
}

#endif