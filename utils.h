#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <cmath>
#include <memory>

using namespace std;
using std::make_shared;
using std::shared_ptr;

double infinity = std::numeric_limits<double>::infinity();
double infinity_float = std::numeric_limits<float>::infinity();


double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

float random_float() {
    return rand() / (RAND_MAX + 1.0);
}

double random_double(double min, double max){
    return min + (max-min) * random_double();
}

float random_float(float min, float max){
    return min + (max-min) * random_float();
}

#endif