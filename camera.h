#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "interval.h"
#include "hittable.h"
#include "hittable_list.h"
#include "color.h"

#include <fstream>
#include <iostream>

class camera {
    public:
        int screen_width = 800;
        double aspect_ratio = 16.0 / 9.0;
        int max_depth = 10;

        void render(const hittable &world){
            initialize();

            // Rendering
            std::ofstream out_file{"out.ppm"};
            out_file << "P3\n" << screen_width << ' ' << screen_height << "\n255\n";

            for(int j = screen_height - 1; j >= 0; j--){
                for(int i = 0; i < screen_width; i++) {
                    auto u = double(i) / (screen_width - 1);
                    auto v = double(j) / (screen_height - 1);
                    ray r(origin, lower_left + u * horizontal + v * vertical - origin);

                    color pixel_color = ray_color(r, max_depth, world);

                    write_color(out_file, pixel_color);
                }
            }
        }

    private:
        int screen_height;
        double viewport_height;
        double viewport_width;
        double focal_length;

        point3 origin;
        vec3 horizontal;
        vec3 vertical;
        vec3 lower_left ;

        void initialize(){
            screen_height = static_cast<int>(screen_width / aspect_ratio);
            viewport_height = 2.0;
            viewport_width = aspect_ratio * viewport_height;
            focal_length = 1.0;

            origin = point3(0,0,0);
            horizontal = vec3(viewport_width, 0, 0);
            vertical = vec3(0, viewport_height, 0);
            lower_left = origin - horizontal/2 - vertical/2 - vec3(0,0,focal_length);
        }

        color ray_color(const ray& r, int depth, const hittable& world){
            if(depth == 0)return color(0.1,0.1,0.1);

            hit_record rec;
            
            if(!world.hit(r, interval(0.00000001, infinity), rec)){
                // Background Color Property
                vec3 unit_v = unit_vector(r.direction());
                auto t = 0.5*(unit_v.y() + 1.0);
                return (1.0-t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
            }

            ray scattered;
            color attenuation;
            double pdf_value;

            rec.mat->scatter(r, rec, attenuation, scattered);

            auto on_light = point3(random_double(-60,60), 200, random_double(-60,60));
            auto to_light = on_light - rec.p;
            auto distance_squared = to_light.length_squared();
            to_light = unit_vector(to_light);

            auto shadow_light = point3(0, 200, 0);
            auto to_shadow_light = unit_vector(shadow_light - rec.p);

            if (dot(to_light, rec.normal) < 0)
                return 0.6 * attenuation * ray_color(scattered, depth - 1, world);

            //double light_area = (343-213)*(332-227);
            double light_area = (120)*(120);
            auto light_cosine = std::fabs(to_light.y());

            if (light_cosine < 0.000001)
                return 0.6 * attenuation * ray_color(scattered, depth - 1, world);

            pdf_value = distance_squared / (light_cosine * light_area);
            
            ray lightray = ray(rec.p, to_light);
            ray shadowray = ray(rec.p, to_shadow_light);
            if(world.hit(shadowray, interval(0.00000001, infinity), rec)){
                return 0.3 * attenuation * ray_color(scattered, depth - 1, world);
            }

            if(world.hit(lightray, interval(0.00000001, infinity), rec)){
                return 0.5 * attenuation * ray_color(scattered, depth - 1, world);
            }

            /*color color_from_scatter = color(1,1,1) - (attenuation * ray_color(lightray, depth-1, world))  pdf_value;

            color next = ray_color(scattered, depth - 1, world);*/

            return attenuation * ray_color(scattered, depth - 1, world) * interval(0.2,1).clamp(pdf_value);
            /*

            */


        }
};



#endif