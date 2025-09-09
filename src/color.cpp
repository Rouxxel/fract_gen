#include "color_rgb.hpp"
#include <cmath>

RGB get_color(int iter, int max_iter) {
    if (iter >= max_iter) return {0, 0, 0}; // black for points inside the set

    double t = (double)iter / max_iter;
    unsigned char r = (unsigned char)(9*(1-t)*t*t*t*255);
    unsigned char g = (unsigned char)(15*(1-t)*(1-t)*t*t*255);
    unsigned char b = (unsigned char)(8.5*(1-t)*(1-t)*(1-t)*t*255);

    return {r, g, b};
}
