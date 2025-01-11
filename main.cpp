#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "color.h"

#include <iostream>
#include <fstream>
#include <vector>


int main(){
    // Screen properties
    const int screen_width = 800;
    const double aspect_ratio = 16.0 / 9.0;
    const int screen_height = static_cast<int>(screen_width / aspect_ratio);

    // Camera properties
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0,0,0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left = origin - horizontal/2 - vertical/2 - vec3(0,0,focal_length);

    // Creating objects
    sphere s1(vec3(0, 0, -5), 2.0);

    vector<sphere> spheres {s1};


    // Rendering
    std::ofstream out_file{"out.ppm"};
    out_file << "P3\n" << screen_width << ' ' << screen_height << "\n255\n";

    for(int j = screen_height - 1; j >= 0; j--){
        for(int i = 0; i < screen_width; i++) {
            auto u = double(i) / (screen_width - 1);
            auto v = double(j) / (screen_height - 1);
            ray r(origin, lower_left + u * horizontal + v * vertical - origin);

            color pixel_color = getColor(r);
            for(auto s : spheres){
                double t = s.intersect(r);

                if(t > 0.0){
                    vec3 n = normalize(r.at(t) - s.center);

                    pixel_color = 0.5 * color(n.x()+1, n.y()+1, n.z()+1);
                }
            }
            write_color(out_file, pixel_color);
        }
    }

    return 0;
}
