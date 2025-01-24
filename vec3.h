#ifndef VEC3_H
#define VEC3_H

#include "utils.h"
#include "vec4.h"

#include <cmath>
#include <iostream>

using namespace std;

class vec3 {
    public:
        double e[3];

        vec3() : e{0,0,0} {};
        vec3(double x, double y, double z) : e{x,y,z} {};


        template <typename VecType>
        vec3(const VecType& v) : e{v.x, v.y, v.z} {}

        double x() { return e[0]; }
        double y() { return e[1]; }
        double z() { return e[2]; }

        vec3 operator-() { return vec3(-e[0], -e[1], -e[2]); }
        double operator[](int i) { return e[i]; }
        
        vec3 operator+=(const vec3 &v){
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        vec3 operator*=(const double t){
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        vec3 operator/=(const double t){
            return *this *= 1/t;
        }

        double length() const {
            return sqrt(length_squared());
        }

        double length_squared() const {
            return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
        }
        
        static vec3 random(double min, double max) {  
            return vec3(
                random_double(min, max),
                random_double(min, max),
                random_double(min, max)
            );
        }
};

std::ostream& operator<<(std::ostream &out, vec3 v){
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]); 
}

vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]); 
}

vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]); 
}

vec3 operator*(double t, const vec3 &v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]); 
}

vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

vec3 operator/(const vec3 &v, double t) {
    return (1/t) * v;
}

double dot(const vec3 &u, const vec3 &v){
    double r = 0;
    r += u.e[0] * v.e[0];
    r += u.e[1] * v.e[1];
    r += u.e[2] * v.e[2];
    return r;
}

vec3 cross(const vec3 &u, const vec3 &v){
    return vec3(
        u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]
    );
}

vec3 unit_vector(const vec3 &v){
    return v / v.length();
}

vec3 random_in_unit_sphere(){
    while(true){
        auto p = vec3::random(-1,1);
        if(p.length_squared() < 1)
            return p;
    }
}

vec3 random_unit_vector(){
    return unit_vector(random_in_unit_sphere());
}

vec3 random_on_hemisphere(const vec3& normal){
    vec3 on_unit_sphere = random_unit_vector();

    if(dot(on_unit_sphere, normal) > 0.0){
        return on_unit_sphere;
    }
    return -on_unit_sphere;
}

// Vector that is specularly reflected when v hits the point with normal n
vec3 reflect(const vec3& v, const vec3& n){
    return v - 2*dot(v,n)*n;
}

// Refraction from raytracing in one weekend, currently using absolute sqrt, not including full refraction
inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat){
    auto cos_theta = fmin(dot(-1 * uv, n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}


using point3 = vec3;
using color = vec3;


#endif