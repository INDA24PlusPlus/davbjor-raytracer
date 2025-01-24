#ifndef RAY4_H
#define RAY4_H

#include "vec4.h"

class ray4 {
    public:
        point4 orig;
        vec4 dir;

        ray4() {};
        ray4(const point4& o,const vec4& d) : orig(o), dir(d) {};

        point4 origin() const { return orig; }
        point4 direction() const { return dir; }

        point4 at(double t) const {
            return orig + t*dir;
        }

        point4 simd_at(double t) const {
            return simd_add_mul(orig, dir, t);
        }
};

#endif