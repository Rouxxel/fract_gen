#include "palette.hpp"
#include "json.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>

using namespace std;
using json = nlohmann::json;

//Load colors from a JSON file into a vector<RGB>
vector<RGB> load_palette(const string& path) {
    ifstream file(path);
    if (!file.is_open()) {
        throw runtime_error("Could not open color palette: " + path);
    }

    json j;
    file >> j;
    file.close();

    vector<RGB> palette;
    for (const auto& c : j["colors"]) {
        if (c.size() >= 3) {
            palette.push_back({ static_cast<unsigned char>(c[0]),
                                static_cast<unsigned char>(c[1]),
                                static_cast<unsigned char>(c[2]) });
        }
    }

    if (palette.empty()) {
        throw runtime_error("Palette is empty or invalid: " + path);
    }

    return palette;
}

//Map iteration count to a color in the palette
RGB get_color_from_palette(int iter, int max_iter, const vector<RGB>& palette) {
    if (iter >= max_iter) return {0, 0, 0}; //inside Mandelbrot set = black
    int idx = (iter * (palette.size() - 1)) / max_iter; //linear mapping
    return palette[idx];
}
