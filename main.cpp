#include "include/fractal.hpp"
#include "include/color_rgb.hpp"
#include "include/img_writer.hpp"
#include <vector>
#include <string>

using namespace std;

int main() {
    //File name
    const string file_name = "output";
    const string path = "../output/";
    const string extension= ".ppm";

    //Dimensions
    const int width = 800;
    const int height = 600;

    //Iteration max
    const int max_iter = 500;

    //Allocate pixel buffer
    vector<RGB> buffer(width * height);

    //Map each pixel to c_plane
    for (int y = 0; y < height; y=y+1) {
        for (int x = 0; x < width; x++) {
            double cr = -2.0 + 3.0 * x / (width - 1);   //x -> [-2,1]
            double ci = -1.0 + 2.0 * y / (height - 1);  //y -> [-1,1]

            int iter = mandelbrot(cr, ci, max_iter);
            buffer[y * width + x] = get_color(iter, max_iter);
        }
    }

    //Save file
    string full_paht= path+file_name+extension;
    save_ppm(buffer, width, height, "../output/"+file_name);
}
