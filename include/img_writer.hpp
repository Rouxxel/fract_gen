#pragma once
#include "color_rgb.hpp"
#include <vector>
#include <string>

using namespace std;

void save_ppm(const vector<RGB>& buffer, int width, int height, const string& filename);
