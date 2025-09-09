#include "img_writer.hpp"
#include <fstream>

using namespace std;

void save_ppm(
    const vector<RGB>& buffer, 
    int width, 
    int height, 
    const string& filename
) {
    ofstream ofs(filename, ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";

    for (const auto& px : buffer) {
        ofs << px.r << px.g << px.b;
    }

    ofs.close();
}
