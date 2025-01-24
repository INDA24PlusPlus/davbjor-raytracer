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

        virtual bool fast_hit(const ray& r, interval ray_t, hit_record& rec) const override;

        virtual bool simd_hit(const ray4& r4, interval ray_t, hit_record4& rec4) const override;
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
        // Reading second hit
        root = (-half_b + sqrtd) / a;
        if(!ray_t.surrounds(root)){
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;

    rec.set_face_normal(r, outward_normal);
   
    rec.mat = mat;
    
    return true;
}


bool sphere::fast_hit(const ray& r, interval ray_t, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    vec4 oc4 = oc;
    vec4 rdir4 = r.direction();

    /*
    float a = simd_length_squared(rdir4);
    auto half_b = dot(oc, r.direction());
    float c = simd_length_squared(oc4) - radius * radius;
    */
    auto a = r.direction().length_squared();
    float half_b = simd_dot(oc4, rdir4);
    auto c = oc.length_squared() - radius * radius;
    float discriminant = half_b*half_b - a*c;
    
    if (discriminant < 0) return false;
    float sqrtd = sqrt(discriminant);

    // Returns the nearest root that lies in the acceptable range
    float root = (-half_b - sqrtd) / a;
    if(!ray_t.surrounds(root)){
        // Reading second hit
        root = (-half_b + sqrtd) / a;
        if(!ray_t.surrounds(root)){
            return false;
        }
    }

    rec.t = root;
    rec.p = r.fast_at(rec.t);
    //vec3 outward_normal = (rec.p - center) / radius;
    rec.normal = (rec.p - center) / radius;
    //rec.normal = simd_mul(simd_minus(vec4(rec.p), vec4(center)), 1/radius);

    //rec.set_face_normal(r, outward_normal);
   
    rec.mat = mat;
    
    return true;
}

bool sphere::simd_hit(const ray4& r4, interval ray_t, hit_record4& rec4) const {
    if(ray_t.max - ray_t.min < 0.001)return false;
    vec4 center4 (center);
    vec4 oc = simd_minus(r4.origin(), center4);

    float a = simd_length_squared(r4.direction());
    float half_b = simd_dot(oc, r4.direction());
    float c = simd_length_squared(oc) - radius * radius;
    float discriminant = half_b*half_b - a*c;
    
    //cout << "hit discriminant " << discriminant <<"\n";

    if (discriminant != 0 && discriminant < 0.001) return false;
    float sqrtd = sqrt(discriminant);

    // Returns the nearest root that lies in the acceptable range
    float root = (-half_b - sqrtd) / a;
    if(!ray_t.surrounds(root)){
        // Reading second hit
        root = (-half_b + sqrtd) / a;
        if(!ray_t.surrounds(root)){
            return false;
        }
    }

    rec4.t = root;
    rec4.p = r4.simd_at(rec4.t);
    //vec4 outward_normal = simd_mul(simd_minus(rec4.p, center4), 1/radius);

    //rec4.set_face_normal(r4, outward_normal);
    rec4.normal = simd_mul(simd_minus(rec4.p, center4), 1/radius);
    rec4.mat = mat;
    
    return true;
}

#endif