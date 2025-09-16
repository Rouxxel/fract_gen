#pragma once
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;

class Logger {
private:
    ofstream log_file;
    mutex mtx; //thread-safe logging

    string current_time() {
        auto now = chrono::system_clock::now();
        time_t t_now = chrono::system_clock::to_time_t(now);
        tm tm_now;
#ifdef _WIN32
        localtime_s(&tm_now, &t_now);
#else
        localtime_r(&t_now, &tm_now);
#endif
        stringstream ss;
        ss << put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

public:
    //Log an entire line at once (single timestamp)
    template <typename T>
    void log_line(const T& msg) {
        lock_guard<mutex> lock(mtx);
        string timestamp = current_time();
        cout << "[" << timestamp << "] | " << msg << endl;
        if (log_file.is_open())
            log_file << "[" << timestamp << "] | " << msg << endl;
    }
    
    Logger(const string& file_path) {
        log_file.open(file_path, ios::out | ios::app);
        if (!log_file.is_open()) {
            cerr << "Failed to open log file: " << file_path << endl;
        }
    }

    ~Logger() {
        if (log_file.is_open()) log_file.close();
    }

    template <typename T>
    Logger& operator<<(const T& msg) {
        lock_guard<mutex> lock(mtx);
        string timestamp = current_time();
        cout << "[" << timestamp << "] | " << msg;
        if (log_file.is_open()) log_file << "[" << timestamp << "] | " << msg;
        return *this;
    }

    //Handle endl
    typedef ostream& (*StreamManipulator)(ostream&);
    Logger& operator<<(StreamManipulator manip) {
        lock_guard<mutex> lock(mtx);
        manip(cout);

        if (log_file.is_open()) manip(log_file);
        return *this;
    }
};
