#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using namespace std;

class vec3 {
    public:
        double e[3];

        vec3() : e{0,0,0} {};
        vec3(double x, double y, double z) : e{x,y,z} {};

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

vec3 normalize(const vec3 &v){
    return v / v.length();
}

using point3 = vec3;
using color = vec3;


#endif