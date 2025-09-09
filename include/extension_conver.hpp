#pragma once
#include "color_rgb.hpp"
#include <vector>
#include <string>

using namespace std;

//Save in PNG, JPG, BMP or TGA
void convert_ppm(const vector<RGB>& buffer, int width, int height, const string& filename);
