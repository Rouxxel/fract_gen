#pragma once

//RGB as in characters
struct RGB {
    unsigned char r, g, b;
};

RGB get_color(int iter, int max_iter);