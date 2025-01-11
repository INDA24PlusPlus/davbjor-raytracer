#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "utils.h"

class material {
    public:
        virtual ~material() = default;

        virtual bool scatter(const ray& r_in, const hit_record& rec, color &attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
    public:
        lambertian(const color& a) : albedo(a) {};

        bool scatter(const ray& r_in, const hit_record& rec, color &attenuation, ray& scattered) const override {
            vec3 scatter_direction = rec.normal + random_unit_vector();
            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    private:
        color albedo;
};

class metal : public material {
    public:
        metal(const color& a) : albedo(a), fuzz(0) {};
        metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {};

        bool scatter(const ray& r_in, const hit_record& rec, color &attenuation, ray& scattered) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz * random_unit_vector());
            attenuation = albedo;
            // Returns if ray was absorbed
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    private:
        color albedo;
        double fuzz;
};


class dielectic : public material {
    public:
        dielectic(double index_of_refraction) : ir(index_of_refraction) {};

        bool scatter(const ray& r_in, const hit_record& rec, color &attenuation, ray& scattered) const override {
            vec3 unit_direction = unit_vector(r_in.direction());
            double refraction_ratio = rec.front_face ? (1.0/ir) : ir;

            // Detect total internal reflection
            double cos_theta = fmin(dot(-1 * unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
            bool internal_refraction = refraction_ratio * sin_theta > 1.0;
            vec3 direction;

            if(internal_refraction || reflectance(cos_theta, refraction_ratio) > random_double())
                direction = reflect(unit_direction, rec.normal);
            else 
                direction = refract(unit_direction, rec.normal, refraction_ratio);

            scattered = ray(rec.p, direction);
            attenuation = color(1,1,1);
            return true;
        }

    private:
        double ir;

        static double reflectance(double cosine, double refraction_index){
            // Approximation for reflactance using Schlick
            auto r0 = (1 - refraction_index) / (1 + refraction_index);
            r0 = r0*r0;
            return r0 + (1-r0) * pow((1-cosine), 5);
        }
};



#endif