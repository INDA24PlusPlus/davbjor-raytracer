//#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "interval.h"
#include "hittable.h"
#include "hittable_list.h"
#include "color.h"

#include <fstream>
#include <iostream>
#include <chrono>

class camera {
    public:
        int screen_width = 1200;
        double aspect_ratio = 16.0 / 9.0;
        int max_depth = 10;
        int iterations_done = 0;

        void render(const hittable &world, const hittable &lights){
            initialize();

            // Rendering
            std::ofstream out_file{"out.ppm"};
            out_file << "P3\n" << screen_width << ' ' << screen_height << "\n255\n";

            vector<vector<color>> grid (screen_height, vector<color> (screen_width));

            auto start = std::chrono::high_resolution_clock::now();
            auto step1 = start;

            int iterations = 15;
            /*vec4 origin4 (origin, 0.0f);
            vec4 vertical4 (vertical.x(), vertical.y(), vertical.z(), 0);
            vec4 horizontal4 (horizontal.x(), horizontal.y(), horizontal.z(), 0);
            vec4 lower_left4 (lower_left.x(), lower_left.y(), lower_left.z(), 0);*/

            for(int k = 0; k < iterations; k++){
                cout << "iteration " << k << "/" << iterations << "\n";
                for(int j = screen_height - 1; j >= 0; j--){
                    for(int i = 0; i < screen_width; i++) {
                        auto u = double(i) / (screen_width  - 1) + random_double(0.000001,0.002) - 0.001;
                        auto v = double(j) / (screen_height - 1) + random_double(0.000001,0.002) - 0.001;
                    
                        ray r(origin, lower_left + u * horizontal + v * vertical - origin);
                        //ray4 r4(origin4, simd_add(simd_add(lower_left4, simd_mul(horizontal4, u)), simd_minus(simd_mul(vertical4, v), origin4)));

                        color pixel_color = ray_color(r, max_depth, world, lights);
                        //color pixel_color = fast_ray_color(r, max_depth, world, lights);
                        //color pixel_color = simd_ray_color(r4, max_depth, world, lights);
                        
                        if(k == 0)
                            grid[j][i] = pixel_color;
                        else
                            grid[j][i] = grid[j][i] + pixel_color;
                        
                    }
                }

                auto step2 = std::chrono::high_resolution_clock::now();
                auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(step2 - step1);
                std::cout << "time: " << diff.count() << " ms" << std::endl;
                swap(step2, step1);
            }


            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            std::cout << "Execution time: " << duration.count() << " ms" << std::endl;
            cout << "------------------ Next Stage -------------------\n\n\n";

            for(int j = screen_height - 1; j >= 0; j--){
                for(int i = 0; i < screen_width; i++) {
                    write_color(out_file, grid[j][i] / iterations);
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

        /*color simd_ray_color(const ray4& r4, int depth, const hittable& world, const hittable& lights){
            //cout << "x: " << iterations_done++ << "\n";
            iterations_done++;
            if(depth <= 0)return color(0,0,0);
            hit_record4 rec4;
            hit_record4 light_rec4;

            bool comment = false;

            bool world_hit = false;
            if(!world.simd_hit(r4, interval(0.00001, infinity), rec4)){
                if(comment)cout << "NO WORLD HIT\n";
                if(lights.simd_hit(r4, interval(0.00001, infinity), light_rec4)){
                    if(comment)cout << "LIGHT HIT\n";
                    return color(10,10,10);
                }
                if(comment)cout << "NO LIGHT HIT\n";
                return color(0,0,0);
            }

            if(comment)cout << "WORLD HIT! t = " << rec4.t << "\n";
            if(lights.simd_hit(r4, interval(0.00001, rec4.t), light_rec4)){
                //cout << "LIGHT HIT2\n" << iterations_done;
                return color(10,10,10);
            }
            if(comment)cout << "NO LIGHT HIT2\n";

            ray4 scattered4;
            color attenuation;

            rec4.mat->scatter4(r4, rec4, attenuation, scattered4);

            if(comment)cout << "SENDING!\n";
            vec3 next = simd_ray_color(scattered4, depth - 1, world, lights);
            vec3 trace = attenuation * next;
            float t = simd_dot(r4.dir, scattered4.dir);
            color res = trace * t;
            return res;
        }*/

        color ray_color(const ray& r, int depth, const hittable& world, const hittable& lights){
            if(depth == 0)return color(0,0,0);

            hit_record lrec;
            hit_record rec;
            
            if(!world.hit(r, interval(0.00000001, infinity), rec)){
                if(lights.hit(r, interval(0.00000001, infinity), lrec)){
                    return color(10,10,10);
                }
                return color(0,0,0);
            }

            if(lights.hit(r, interval(0.00000001, rec.t), lrec)){
                return color(10,10,10);
            }

            ray scattered;
            color attenuation;

            rec.mat->scatter(r, rec, attenuation, scattered);

            return attenuation * ray_color(scattered, depth - 1, world, lights) * dot(r.dir, scattered.dir);
        }


        color fast_ray_color(const ray& r, int depth, const hittable& world, const hittable& lights){
            if(depth == 0)return color(0,0,0);

            hit_record lrec;
            hit_record rec;
            
            if(!world.fast_hit(r, interval(0.00000001, infinity), rec)){
                if(lights.fast_hit(r, interval(0.00000001, infinity), lrec)){
                    return color(10,10,10);
                }
                return color(0,0,0);
            }

            if(lights.fast_hit(r, interval(0.00000001, rec.t), lrec)){
                return color(10,10,10);
            }

            ray scattered;
            color attenuation;

            rec.mat->fast_scatter(r, rec, attenuation, scattered);

            return attenuation * fast_ray_color(scattered, depth - 1, world, lights) * simd_dot(vec4(r.dir,0), vec4(scattered.dir,0));
        }
};



//#endif