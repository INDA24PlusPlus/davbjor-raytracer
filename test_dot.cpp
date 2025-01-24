#include "vec4.h"

#include <iostream>
#include <vector>


int main(){
    vec4 a (1, 0, 3, 1);
    vec4 b (2, 100, 1, 4);

    cout << dot(a,b) << "\n";
    cout << simd_dot(a,b) << "\n";

    return 0;
}