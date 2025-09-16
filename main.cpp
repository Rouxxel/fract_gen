#include "include/fractal.hpp"
#include "include/color_rgb.hpp"
#include "include/img_writer.hpp"
#include "include/extension_conver.hpp"
#include "include/json.hpp"
#include "include/palette.hpp"
#include "include/custom_logger.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <omp.h>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;
using json = nlohmann::json;

int main() {
    // ----------------------
    // Load JSON config and set logger
    // ----------------------
    //Get current date/time as YYYY-MM-DD_HH-MM-SS
    auto date_now = chrono::system_clock::now();
    time_t t_now = chrono::system_clock::to_time_t(date_now);
    tm tm_now;
    #ifdef _WIN32
        localtime_s(&tm_now, &t_now);   //Windows-safe
    #else
        localtime_r(&t_now, &tm_now);   //Linux/Unix-safe
    #endif

    ostringstream oss;
    oss << "fractal_" << put_time(&tm_now, "%Y-%m-%d_%H-%M-%S") << ".log";

    //Create logger
    Logger log("../output/" + oss.str());
    log << "Logging started..." << endl;
    log << "Logs being written to ../output/" << endl;

    //Configuration
    string config_path = "../assets/config/default_config.json";
    json config;

    ifstream config_file(config_path);
    if (config_file.is_open()) {
        config_file >> config;
        config_file.close();
    } else {
        log << "Warning: Could not open config file: " << config_path << endl;
    }

    // ----------------------
    // Set defaults from JSON
    // ----------------------
    string file_name = config["mutable"].value("file_name", "output");
    string conver_ext = config["mutable"].value("conver_extension", ".png");
    int width = config["mutable"].value("width.px", 800);
    int height = config["mutable"].value("height.px", 600);
    int max_iter = config["mutable"].value("max_iter", 500);

    int num_threads = config["parallel_section"]["num_of_threads"];
    bool parallelize = config["parallel_section"]["parallelize"];

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
    string palette_extension = config["color_palettes"].value("palette_extension", ".json");

    log << "Enter palette JSON file name in '../assets/palettes/' (press Enter for default): ";
    getline(cin, palette_json);

    bool use_palette = false;
    string full_palette_path = palette_path + palette_json + palette_extension;
    if (!palette_json.empty()) {
        try {
            palette = load_palette(full_palette_path);
            use_palette = true;
        } catch (const exception& e) {
            log << "Failed to load palette, using default procedural colors. (" << e.what() << ")\n";
            log << "Ensure the entered palette is saved in '../assets/palettes/' !!!";
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////
    // ----------------------
    // Ask user (can override defaults)
    // ----------------------
    log << "Enter file name (default = " << file_name << "): ";
    getline(cin, input);
    if (!input.empty()) file_name = input;

    log << "Enter conversion extension (default = " << conver_ext << "): ";
    getline(cin, input);
    if (!input.empty()) conver_ext = input;

    log << "Enter width (default = " << width << "): ";
    getline(cin, input);
    if (!input.empty()) width = stoi(input);

    log << "Enter height (default = " << height << "): ";
    getline(cin, input);
    if (!input.empty()) height = stoi(input);

    log << "Enter max iterations (default = " << max_iter << "): ";
    getline(cin, input);
    if (!input.empty()) max_iter = stoi(input);

    // ----------------------
    // Confirm settings
    // ----------------------
    log << "\nUsing settings:\n";
    log << "File: " << path + file_name + extension << endl;
    log << "Convert to: " << conver_ext << endl;
    log << "Resolution: " << width << "x" << height << "px (width x height)"<< endl;
    log << "Max iterations: " << max_iter << endl;
    log << endl;
    if (use_palette){
        log << "Using palette from: " << palette_path << endl;
    } else {
        log << "Using default procedural colors." << endl;
    } 
    log << endl;

    log << "Running script now..." << endl;

    ////////////////////////////////////////////////////////////////////////////////////////////
    // ----------------------
    // Start process
    // ----------------------
    //Allocate pixel buffer
    vector<RGB> buffer(width * height);

    //Map each pixel to c_plane
    if (parallelize) omp_set_num_threads(num_threads);
    #pragma omp parallel for collapse(2) schedule(dynamic) if(parallelize)
    for (int y = 0; y < height; y=y+1) {
        for (int x = 0; x < width; x++) {
            double cr = -2.0 + 3.0 * x / (width - 1);   //x -> [-2,1]
            double ci = -1.0 + 2.0 * y / (height - 1);  //y -> [-1,1]

            int iter = mandelbrot(cr, ci, max_iter);
            buffer[y * width + x] = use_palette
                ? get_color_from_palette(iter, max_iter, palette)
                : get_color(iter, max_iter);
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
    log << "Image '" << file_name << "' saved to '" << path << "' as .ppm and " << conver_ext << endl;
}
