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
#include <cstdlib>
#include <ctime>

using namespace std;
using json = nlohmann::json;

int main() {
    // ----------------------
    // Create logger with timestamped file
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
    Logger log("../logs/" + oss.str());
    log.log_line("Logging started...");
    log.log_line("Logs being written to ../logs/");

    //Configuration
    string config_path = "../assets/config/default_config.json";
    json config;

    ifstream config_file(config_path);
    if (config_file.is_open()) {
        config_file >> config;
        config_file.close();
    } else {
        log.log_line("Warning: Could not open config file: " + config_path);
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

    double center_x = config["view_customization"]["center_x"];
    double center_y = config["view_customization"]["center_y"];
    double zoom = config["view_customization"]["zoom"];
    bool randomize_view = config["view_customization"]["random_view"];

    string input;
    
    ////////////////////////////////////////////////////////////////////////////////////////////
    // ----------------------
    // Load optional color palette
    // ----------------------
    vector<RGB> palette;
    string palette_path = config["color_palettes"].value("palettes_path", "../assets/palettes/");
    string palette_json;
    string palette_extension = config["color_palettes"].value("palette_extension", ".json");

    log.log_line("Enter palette JSON file name in '../assets/palettes/' (press Enter for default): ");
    getline(cin, palette_json);

    bool use_palette = false;
    string full_palette_path = palette_path + palette_json + palette_extension;
    if (!palette_json.empty()) {
        try {
            palette = load_palette(full_palette_path);
            use_palette = true;
        } catch (const exception& e) {
            log.log_line("Failed to load palette, using default procedural colors. (" + string(e.what()) + ")");
            log.log_line("Ensure the entered palette is saved in '../assets/palettes/' !!!");
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////
    // ----------------------
    // Ask user (can override defaults)
    // ----------------------
    log.log_line("Enter file name (default = " + file_name + "): ");
    getline(cin, input);
    if (!input.empty()) file_name = input;

    log.log_line("Enter conversion extension (default = " + conver_ext + "): ");
    getline(cin, input);
    if (!input.empty()) conver_ext = input;

    log.log_line("Enter width (default = " + to_string(width) + "): ");
    getline(cin, input);
    if (!input.empty()) width = stoi(input);

    log.log_line("Enter height (default = " + to_string(height) + "): ");
    getline(cin, input);
    if (!input.empty()) height = stoi(input);

    log.log_line("Enter max iterations (default = " + to_string(max_iter) + "): ");
    getline(cin, input);
    if (!input.empty()) max_iter = stoi(input);

    // ----------------------
    // Fractal view customization
    // ----------------------
    log.log_line("Do you want a random view? (y/n, default = n): ");
    getline(cin, input);
    if (!input.empty() && (input == "y" || input == "Y")) randomize_view = true;

    if (randomize_view) {
        srand(time(nullptr));
        center_x = -2.0 + 3.0 * rand() / double(RAND_MAX);  // [-2, 1]
        center_y = -1.5 + 3.0 * rand() / double(RAND_MAX);  // [-1.5, 1.5]
        zoom = 0.5 + 4.0 * rand() / double(RAND_MAX);       // random zoom
        log.log_line("Random view selected:");
    } else {
        log.log_line("Enter center X (default = -0.5): ");
        getline(cin, input);
        if (!input.empty()) center_x = stod(input);

        log.log_line("Enter center Y (default = 0.0): ");
        getline(cin, input);
        if (!input.empty()) center_y = stod(input);

        log.log_line("Enter zoom (default = 1.0, higher = zoom in): ");
        getline(cin, input);
        if (!input.empty()) zoom = stod(input);
    }

    // ----------------------
    // Confirm settings
    // ----------------------
    log.log_line("\nUsing settings:");
    log.log_line("File: " + path + file_name + extension);
    log.log_line("Convert to: " + conver_ext);
    log.log_line("Resolution: " + to_string(width) + "x" + to_string(height) + "px");
    log.log_line("Max iterations: " + to_string(max_iter));
    log.log_line("Center: (" + to_string(center_x) + ", " + to_string(center_y) + ")");
    log.log_line("Zoom: " + to_string(zoom));

    log.log_line(use_palette ? "Using custom palette." : "Using procedural colors.");

    if (use_palette) {
        log.log_line("Using palette from: " + palette_path);
    } else {
        log.log_line("Using default procedural colors.");
    }

    log.log_line("Running script now...");

    ////////////////////////////////////////////////////////////////////////////////////////////
    // ----------------------
    // Start process
    // ----------------------
    //Allocate pixel buffer
    vector<RGB> buffer(width * height);

    //Map each pixel to c_plane
    if (parallelize) omp_set_num_threads(num_threads);
    #pragma omp parallel for collapse(2) schedule(dynamic) if(parallelize)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Dynamic mapping: depends on center and zoom
            double cr = center_x + (x - width / 2.0) * (3.0 / width) / zoom;
            double ci = center_y + (y - height / 2.0) * (2.0 / height) / zoom;

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
    string full_path = path + file_name + extension;
    save_ppm(buffer, width, height, "../output/" + file_name);

    //Convert file
    convert_ppm(buffer, width, height, path + file_name + conver_ext);
    log.log_line("Image '" + file_name + "' saved to '" + path + "' as .ppm and " + conver_ext);

    return 0;
}
