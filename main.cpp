#include "include/fractal.hpp"
#include "include/color_rgb.hpp"
#include "include/img_writer.hpp"
#include "include/extension_conver.hpp"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

int main() {
    //Default values
    string file_name = "output";
    string path = "../output/";
    string extension = ".ppm";
    string conver_ext = ".png";
    int width = 800;
    int height = 600;
    int max_iter = 500;

    string input;
    
    //Ask user
    cout << "Enter file name (default = " << file_name << "): ";
    getline(cin, input);
    if (!input.empty()) file_name = input;

    cout << "Enter conversion extension (default = " << conver_ext << "): ";
    getline(cin, input);
    if (!input.empty()) conver_ext = input;

    cout << "Enter width (default = " << width << "): ";
    getline(cin, input);
    if (!input.empty()) width = stoi(input);

    cout << "Enter height (default = " << height << "): ";
    getline(cin, input);
    if (!input.empty()) height = stoi(input);

    cout << "Enter max iterations (default = " << max_iter << "): ";
    getline(cin, input);
    if (!input.empty()) max_iter = stoi(input);

    // Confirm settings
    cout << "\nUsing settings:\n";
    cout << "File: " << path + file_name + extension << endl;
    cout << "Convert to: " << conver_ext << endl;
    cout << "Resolution: " << width << "x" << height << endl;
    cout << "Max iterations: " << max_iter << endl;


    ////////////////////////////////////////////////////////////////////////////////////////////
    //Start process
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

    ////////////////////////////////////////////////////////////////////////////////////////////
    //Save file
    string full_paht= path+file_name+extension;
    save_ppm(buffer, width, height, "../output/"+file_name);

    //Convert file
    convert_ppm(buffer, width, height, path + file_name + conver_ext);
    cout << "Image saved to: " << path << file_name << "as .ppm and " << conver_ext << endl;
}
