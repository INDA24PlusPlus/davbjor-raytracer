#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "vec3.h"
#include "sphere.h"
#include "hittable.h"

#include <vector>
#include <cmath>

using namespace std;

class hittable_list : public hittable {
    public:
        vector<shared_ptr<hittable>> objects;

        hittable_list() {};

        void clear(){ objects.clear(); }

        void add(shared_ptr<hittable> object) { objects.push_back(object); }

        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
};

bool hittable_list::hit(const ray& r, interval ray_t, hit_record& rec) const {
    bool hit_anything = false;

    for(const auto& object : objects){
        if(object->hit(r, ray_t, rec)) {
            hit_anything = true;
            ray_t.max = rec.t;
        }
    }

    return hit_anything;
}

#endif