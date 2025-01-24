#ifndef RAY_H
#define RAY_H

#include "vec3.h"
#include "vec3.h"

class ray {
    public:
        point3 orig;
        vec3 dir;

        ray() {};
        ray(const point3& o,const vec3& d) : orig(o), dir(d) {};
        ray(const point3& o,const vec4& d) : orig(o), dir(vec3(d.x, d.y, d.z)) {};

        point3 origin() const { return orig; }
        point3 direction() const { return dir; }

        point3 at(double t) const {
            return orig + t*dir;
        }

        point3 fast_at(double t) const {
            return simd_add_mul(vec4(orig), vec4(dir), t);
        }
};

#endif