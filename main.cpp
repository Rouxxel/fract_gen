#include "include/fractal.hpp"
#include "include/color_rgb.hpp"
#include "include/img_writer.hpp"
#include "include/extension_conver.hpp"
#include "include/json.hpp"
#include "include/palette.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
using json = nlohmann::json;

int main() {
    // ----------------------
    // Load JSON config
    // ----------------------
    string config_path = "../assets/config/default_config.json";
    json config;

    ifstream config_file(config_path);
    if (config_file.is_open()) {
        config_file >> config;
        config_file.close();
    } else {
        cerr << "Warning: Could not open config file: " << config_path << endl;
    }

    // ----------------------
    // Set defaults from JSON
    // ----------------------
    string file_name = config["mutable"].value("file_name", "output");
    string conver_ext = config["mutable"].value("conver_extension", ".png");
    int width = config["mutable"].value("width.px", 800);
    int height = config["mutable"].value("height.px", 600);
    int max_iter = config["mutable"].value("max_iter", 500);

    string extension = config["unmutable"].value("default_extension", ".ppm");
    string path = config["unmutable"].value("default_output_dir", "../output/");

    string input;
    
    ////////////////////////////////////////////////////////////////////////////////////////////
    // ----------------------
    // Load optional color palette
    // ----------------------
    vector<RGB> palette;
    string palette_path = config["color_palettes"].value("palettes_path", "../assets/palettes/");
    string palette_json;

    cout << "Enter palette JSON file name in '../assets/palettes/' (press Enter for default): ";
    getline(cin, palette_json);

    bool use_palette = false;
    string full_palette_path = palette_path + palette_json;
    if (!palette_json.empty()) {
        try {
            palette = load_palette(full_palette_path);
            use_palette = true;
        } catch (const exception& e) {
            cerr << "Failed to load palette, using default procedural colors. (" << e.what() << ")\n";
            cerr << "Ensure the entered palette is saved in '../assets/palettes/' !!!";
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////
    // ----------------------
    // Ask user (can override defaults)
    // ----------------------
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

    // ----------------------
    // Confirm settings
    // ----------------------
    cout << "\nUsing settings:\n";
    cout << "File: " << path + file_name + extension << endl;
    cout << "Convert to: " << conver_ext << endl;
    cout << "Resolution: " << width << "x" << height << "px (width x height)"<< endl;
    cout << "Max iterations: " << max_iter << endl;
    cout << endl;
    if (use_palette){
        cout << "Using palette from: " << palette_path << endl;
    } else {
        cout << "Using default procedural colors." << endl;
    }
    cout << endl;

    cout << "Running script now..." << endl;

    ////////////////////////////////////////////////////////////////////////////////////////////
    // ----------------------
    // Start process
    // ----------------------
    //Allocate pixel buffer
    vector<RGB> buffer(width * height);

    //Map each pixel to c_plane
    for (int y = 0; y < height; y=y+1) {
        for (int x = 0; x < width; x++) {
            double cr = -2.0 + 3.0 * x / (width - 1);   //x -> [-2,1]
            double ci = -1.0 + 2.0 * y / (height - 1);  //y -> [-1,1]

            int iter = mandelbrot(cr, ci, max_iter);
            if (use_palette) {
                buffer[y * width + x] = get_color_from_palette(iter, max_iter, palette);
            } else {
                buffer[y * width + x] = get_color(iter, max_iter);
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////
    // ----------------------
    // Save file
    // ----------------------
    string full_paht= path+file_name+extension;
    save_ppm(buffer, width, height, "../output/"+file_name);

    //Convert file
    convert_ppm(buffer, width, height, path + file_name + conver_ext);
    cout << "Image '" << file_name << "' saved to '" << path << "' as .ppm and " << conver_ext << endl;
}
