#include "fractal.hpp"
#include <cmath>

int mandelbrot(double c_real, double c_imaginary, int max_iter) {
    double z_real = 0.0, z_imaginary = 0.0;
    int iter = 0;

    while (pow(z_real,2) + pow(z_imaginary,2) <= 4.0 && iter < max_iter) {
        double temp = pow(z_real,2) - pow(z_imaginary,2) + c_real;

        z_imaginary = (2.0 * z_real * z_imaginary) + c_imaginary;
        z_real = temp;
        iter = iter + 1;
    }

    return iter;
}
