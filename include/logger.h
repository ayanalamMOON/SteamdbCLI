#pragma once
#include <string>
#include <fstream>
#include <mutex>

class Logger {
public:
    void init(const std::string& filename);
    void info(const std::string& message);
    void error(const std::string& message);
    void debug(const std::string& message);
    
private:
    std::ofstream logFile;
    std::mutex logMutex;
    void log(const std::string& level, const std::string& message);
};

void Logger::init(const std::string& filename) {
    std::lock_guard<std::mutex> lock(logMutex);
    logFile.open(filename, std::ios::out | std::ios::app);
    if (!logFile.is_open()) {
        throw std::runtime_error("Failed to open log file: " + filename);
    }
}

void Logger::info(const std::string& message) {
    log("INFO", message);
}

void Logger::error(const std::string& message) {
    log("ERROR", message);
}

void Logger::debug(const std::string& message) {
    log("DEBUG", message);
}

void Logger::log(const std::string& level, const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    logFile << "[" << level << "] " << message << std::endl;
}
