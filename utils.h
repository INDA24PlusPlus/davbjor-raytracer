#ifndef UTILS_H
#define UTILS_H

#include "vec3.h"

#include <vector>
#include <cmath>
#include <memory>

using namespace std;
using std::make_shared;
using std::shared_ptr;

double infinity = std::numeric_limits<double>::infinity();

double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

double random_double(double min, double max){
    return min + (max-min) * random_double();
}

#endif