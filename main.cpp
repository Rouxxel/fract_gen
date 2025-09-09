#include "include/fractal.hpp"
#include <iostream>

int main() {
    int max_iter = 1000;

    //Test
    double test_points[3][2] = {
        {0.0, 0.0},    //center
        {-2.0, 1.0},   //top-left
        {0.3, 0.5}     //somewhere in the set
    };

    for (auto &p : test_points) {
        int iter = mandelbrot(p[0], p[1], max_iter);
        std::cout << "Point (" << p[0] << ", " << p[1] << ") -> Iterations: " << iter << "\n";
    }
}
