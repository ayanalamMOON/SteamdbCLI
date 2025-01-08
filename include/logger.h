#pragma once
#include <string>
#include <fstream>
#include <mutex>

// Logger class to handle logging messages to a file
class Logger {
public:
    // Initialize the logger with a file name
    void init(const std::string& filename);
    
    // Log an info message
    void info(const std::string& message);
    
    // Log an error message
    void error(const std::string& message);
    
    // Log a debug message
    void debug(const std::string& message);
    
private:
    std::ofstream logFile; // Log file stream
    std::mutex logMutex; // Mutex to ensure thread-safe logging
    
    // Log a message with a specific level
    void log(const std::string& level, const std::string& message);
};

// Initialize the logger with a file name
void Logger::init(const std::string& filename) {
    std::lock_guard<std::mutex> lock(logMutex);
    logFile.open(filename, std::ios::out | std::ios::app);
    if (!logFile.is_open()) {
        throw std::runtime_error("Failed to open log file: " + filename);
    }
}

// Log an info message
void Logger::info(const std::string& message) {
    log("INFO", message);
}

// Log an error message
void Logger::error(const std::string& message) {
    log("ERROR", message);
}

// Log a debug message
void Logger::debug(const std::string& message) {
    log("DEBUG", message);
}

// Log a message with a specific level
void Logger::log(const std::string& level, const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    if (logFile.is_open()) {
        logFile << "[" << level << "] " << message << std::endl;
    } else {
        throw std::runtime_error("Log file is not open");
    }
}
