#pragma once
#include "color_rgb.hpp"
#include <vector>
#include <string>

using namespace std;

//Load palette from a JSON file
vector<RGB> load_palette(const string& path);

//Map iteration count to a color from the palette
RGB get_color_from_palette(int iter, int max_iter, const vector<RGB>& palette);
