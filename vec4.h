#ifndef VEC4_H
#define VEC4_H

#include "utils.h"
#include "vec3.h"

#include <cmath>
#include <iostream>
#include <xmmintrin.h>
#include <immintrin.h>
#include <cmath>

using namespace std;

class vec4 {
    public:
        float x, y, z, w;

        vec4() : vec4(0,0,0,0) {};
        vec4(float x, float y, float z, float a) : x(x),y(y),z(z),w(w) {};

        template <typename VecType>
        vec4(const VecType& v, float w = 0.0f) : x(v.e[0]), y(v.e[1]), z(v.e[2]), w(w) {}

        vec4 operator-() { return vec4(-x, -y, -z, -w); }

        float operator[](int i) { 
            if(i==0)return x; 
            if(i==1)return y; 
            if(i==2)return z; 
            return w;
        }
        
        vec4 operator+=(const vec4 &v){
            x += v.x;
            y += v.y;
            z += v.z;
            w += v.w;
            return *this;
        }

        vec4 operator*=(const float t){
            x *= t;
            y *= t;
            z *= t;
            w *= t;
            return *this;
        }

        vec4 operator/=(const float t){
            return *this *= 1/t;
        }

        float length() const {
            return sqrt(length_squared());
        }

        float length_squared() const {
            return x * x + y * y + z * z + w * w;
        }
        
        static vec4 random(float min, float max) {  
            return vec4(
                random_float(min, max),
                random_float(min, max),
                random_float(min, max),
                random_float(min, max)
            );
        }
};

std::ostream& operator<<(std::ostream &out, vec4 v){
    return out << v.x << ' ' << v.y << ' ' << v.z;
}

vec4 operator+(const vec4 &u, const vec4 &v) {
    return vec4(u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w); 
}

vec4 operator-(const vec4 &u, const vec4 &v) {
    return vec4(u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w); 
}

vec4 operator*(const vec4 &u, const vec4 &v) {
    return vec4(u.x * v.x, u.y * v.y, u.z * v.z, u.w * v.w); 
}

vec4 operator*(float t, const vec4 &v) {
    return vec4(t * v.x, t * v.y, t * v.z, t * v.w);
}

vec4 operator*(const vec4 &v, float t) {
    return t * v;
}

vec4 operator/(const vec4 &v, float t) {
    return (1/t) * v;
}

float dot(const vec4 &u, const vec4 &v){
    float r = 0;
    r += u.x * v.x;
    r += u.y * v.y;
    r += u.z * v.z;
    r += u.w * v.w;
    return r;
}

vec4 cross(const vec4 &u, const vec4 &v){
    return vec4(
        u.y * v.z - u.z * v.y,
        u.z * v.x - u.x * v.z,
        u.x * v.y - u.y * v.x,
        1
    );
}

vec4 unit_vector(const vec4 &v){
    return v / v.length();
}

vec4 random_in_unit_sphere_4(){
    while(true){
        auto p = vec4::random(-1,1);
        if(p.length_squared() < 1)
            return p;
    }
}

vec4 random_unit_vector_4(){
    return unit_vector(random_in_unit_sphere_4());
}

vec4 random_on_hemisphere(const vec4& normal){
    vec4 on_unit_sphere = random_unit_vector_4();

    if(dot(on_unit_sphere, normal) > 0.0){
        return on_unit_sphere;
    }
    return -on_unit_sphere;
}

// Vector that is specularly reflected when v hits the point with normal n
vec4 reflect(const vec4& v, const vec4& n){
    return v - 2*dot(v,n)*n;
}

// a + b
vec4 simd_add(vec4 a, vec4 b) {
    const __m128 xmm_a = _mm_load_ps(&a.x);
    const __m128 xmm_b = _mm_load_ps(&b.x);
    const __m128 xmm_res = _mm_add_ps(xmm_a, xmm_b);
    vec4 res;
    _mm_store_ps(&res.x, xmm_res);
    return res;
}

// a - b
vec4 simd_minus(vec4 a, vec4 b) {
    const __m128 xmm_a = _mm_load_ps(&a.x);
    const __m128 xmm_b = _mm_load_ps(&b.x);
    const __m128 xmm_res = _mm_sub_ps(xmm_a, xmm_b);
    vec4 result;
    _mm_store_ps(&result.x, xmm_res);
    return result;
}

// (a*t) + b
vec4 simd_add_mul(vec4 a, float t, vec4 b) {
    const __m128 xmm_a = _mm_load_ps(&a.x); 
    const __m128 xmm_t = _mm_set1_ps(t);
    const __m128 xmm_b = _mm_load_ps(&b.x);

    const __m128 xmm_mul = _mm_mul_ps(xmm_a, xmm_t);
    const __m128 xmm_res = _mm_add_ps(xmm_mul, xmm_b);

    vec4 result;
    _mm_store_ps(&result.x, xmm_res);
    return result;
}

// a + (b*t)
vec4 simd_add_mul(vec4 a, vec4 b, float t) {
    const __m128 xmm_a = _mm_load_ps(&a.x); 
    const __m128 xmm_b = _mm_load_ps(&b.x);
    const __m128 xmm_t = _mm_set1_ps(t);

    const __m128 xmm_mul = _mm_mul_ps(xmm_b, xmm_t);
    const __m128 xmm_res = _mm_add_ps(xmm_a, xmm_mul);

    vec4 result;
    _mm_store_ps(&result.x, xmm_res);
    return result;
}

// (a*t) - b
vec4 simd_minus_mul(vec4 a, float t, vec4 b) {
    const __m128 xmm_a = _mm_load_ps(&a.x); 
    const __m128 xmm_t = _mm_set1_ps(t);
    const __m128 xmm_b = _mm_load_ps(&b.x);

    const __m128 xmm_mul = _mm_mul_ps(xmm_a, xmm_t);
    const __m128 xmm_res = _mm_sub_ps(xmm_mul, xmm_b);

    vec4 result;
    _mm_store_ps(&result.x, xmm_res);
    return result;
}

// a - (b*t)
vec4 simd_minus_mul(vec4 a, vec4 b, float t) {
    const __m128 xmm_a = _mm_load_ps(&a.x); 
    const __m128 xmm_b = _mm_load_ps(&b.x);
    const __m128 xmm_t = _mm_set1_ps(t);

    const __m128 xmm_mul = _mm_mul_ps(xmm_b, xmm_t);
    const __m128 xmm_res = _mm_sub_ps(xmm_a, xmm_mul);

    vec4 result;
    _mm_store_ps(&result.x, xmm_res);
    return result;
}

// a * t
vec4 simd_mul(vec4 a, float t) {
    const __m128 xmm_a = _mm_load_ps(&a.x);
    const __m128 xmm_t = _mm_set1_ps(t);   

    const __m128 xmm_res = _mm_mul_ps(xmm_a, xmm_t);

    vec4 result;
    _mm_store_ps(&result.x, xmm_res);
    return result;
}

// a * b
vec4 simd_mul(vec4 a, vec4 b) {
    const __m128 xmm_a = _mm_load_ps(&a.x);
    const __m128 xmm_b = _mm_load_ps(&b.x);

    const __m128 xmm_res = _mm_mul_ps(xmm_a, xmm_b);

    vec4 result;
    _mm_store_ps(&result.x, xmm_res);
    return result;
}

vec4 simd_normalize(vec4 a) {
    const __m128 xmm_a = _mm_load_ps(&a.x);
    const __m128 square_magnitude = _mm_dp_ps(xmm_a, xmm_a, 0xff);
    const __m128 inv_magnitude = _mm_rsqrt_ps(square_magnitude);
    const __m128 normalized_a = _mm_mul_ps(xmm_a, inv_magnitude);
    vec4 res;
    _mm_store_ps(&res.x, normalized_a);
    return res;
}

float simd_dot(vec4 a, vec4 b){
    const __m128 xmm_a = _mm_load_ps(&a.x);
    const __m128 xmm_b = _mm_load_ps(&b.x);
    const __m128 xmm_res = _mm_dp_ps(xmm_a, xmm_b, 0xff);
    return _mm_cvtss_f32(xmm_res);
}

float simd_length_squared(vec4 a) {
    const __m128 xmm_a = _mm_load_ps(&a.x);
    const __m128 xmm_res = _mm_dp_ps(xmm_a, xmm_a, 0xff);
    return _mm_cvtss_f32(xmm_res);
}

//v - 2*dot(v,n)*n;
vec4 simd_reflect(const vec4& v, const vec4& n){
    const __m128 xmm_v = _mm_load_ps(&v.x);
    const __m128 xmm_n = _mm_load_ps(&n.x);
    const __m128 xmm_dot = _mm_dp_ps(xmm_v, xmm_n, 0xff);

    float t = _mm_cvtss_f32(xmm_dot);
    const __m128 xmm_t = _mm_set1_ps(t * 2);
    const __m128 xmm_mul = _mm_mul_ps(xmm_n, xmm_t);

    const __m128 xmm_res = _mm_sub_ps(xmm_v, xmm_mul);
    vec4 result;
    _mm_store_ps(&result.x, xmm_res);
    return result;
}

using point4 = vec4;
using color4 = vec4;


#endif