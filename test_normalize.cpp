#include "vec4.h"

#include <iostream>
#include <vector>


int main(){
    vec4 a (1, 0, 3, 1);
    vec4 b (5, 3, 2, 1);

    cout << unit_vector(a) << "\n";
    cout << simd_normalize(a) << "\n";

    cout << unit_vector(b) << "\n";
    cout << simd_normalize(b) << "\n";
    return 0;
}