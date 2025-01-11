#ifndef HITTABLE_H
#define HITTABLE_H

#include "vec3.h"
#include "interval.h"

class material;

class hit_record {
    public:
        point3 p;
        vec3 normal;
        double t;

        shared_ptr<material> mat;
};

class hittable {
    public:
        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif