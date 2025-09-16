#pragma once
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>

using namespace std;

class Logger {
private:
    ofstream log_file;
    mutex mtx; //thread-safe logging

public:
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
        cout << msg;
        if (log_file.is_open()) log_file << msg;
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
