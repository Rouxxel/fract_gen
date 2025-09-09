#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "color_rgb.hpp"
#include "extension_conver.hpp"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

void convert_ppm(const vector<RGB>& buffer, int width, int height, const string& filename) {
    //Convert RGB vector to raw data
    vector<unsigned char> raw;
    raw.reserve(width * height * 3);
    for (const auto& px : buffer) {
        raw.push_back(px.r);
        raw.push_back(px.g);
        raw.push_back(px.b);
    }

    //Find extension
    string ext;
    size_t dot = filename.find_last_of('.');
    if (dot != string::npos) {
        ext = filename.substr(dot + 1);
        for (auto &c : ext) c = tolower(c); //convert to lowercase
    } else {
        cerr << "No file extension found!" << endl;
        return;
    }

    //Call correct stb function
    if (ext == "jpg" || ext == "jpeg") {
        stbi_write_jpg(filename.c_str(), width, height, 3, raw.data(), 100); // quality 100
    } else if (ext == "png") {
        stbi_write_png(filename.c_str(), width, height, 3, raw.data(), width * 3);
    } else if (ext == "bmp") {
        stbi_write_bmp(filename.c_str(), width, height, 3, raw.data());
    } else if (ext == "tga") {
        stbi_write_tga(filename.c_str(), width, height, 3, raw.data());
    } else {
        cerr << "Unsupported file extension: " << ext << endl;
    }
}
