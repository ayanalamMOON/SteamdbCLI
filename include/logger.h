#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <execinfo.h>
#include <cxxabi.h>

// Logger class to handle logging messages to a file
class Logger {
public:
    // Initialize the logger with a file name
    void init(const std::string& filename);
    
    // Log an info message
    void info(const std::string& message);
    
    // Log a warning message
    void warning(const std::string& message);
    
    // Log an error message
    void error(const std::string& message, const std::string& functionName, const std::string& fileName, int lineNumber);
    
    // Log a debug message
    void debug(const std::string& message);
    
private:
    std::ofstream logFile; // Log file stream
    std::mutex logMutex; // Mutex to ensure thread-safe logging
    
    // Log a message with a specific level
    void log(const std::string& level, const std::string& message);
    
    // Get the current timestamp
    std::string getCurrentTimestamp();
    
    // Get the stack trace
    std::string getStackTrace();
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

// Log a warning message
void Logger::warning(const std::string& message) {
    log("WARNING", message);
}

// Log an error message
void Logger::error(const std::string& message, const std::string& functionName, const std::string& fileName, int lineNumber) {
    std::ostringstream oss;
    oss << message << " [Function: " << functionName << ", File: " << fileName << ", Line: " << lineNumber << "]";
    log("ERROR", oss.str());
}

// Log a debug message
void Logger::debug(const std::string& message) {
    log("DEBUG", message);
}

// Log a message with a specific level
void Logger::log(const std::string& level, const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    if (logFile.is_open()) {
        logFile << "[" << getCurrentTimestamp() << "] [" << level << "] " << message << std::endl;
        if (level == "ERROR") {
            logFile << getStackTrace() << std::endl;
        }
    } else {
        throw std::runtime_error("Log file is not open");
    }
}

// Get the current timestamp
std::string Logger::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// Get the stack trace
std::string Logger::getStackTrace() {
    const int maxFrames = 100;
    void* addrlist[maxFrames + 1];
    int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));
    if (addrlen == 0) {
        return "  <empty, possibly corrupt>";
    }
    char** symbollist = backtrace_symbols(addrlist, addrlen);
    std::ostringstream oss;
    for (int i = 4; i < addrlen; i++) {
        std::string symbol(symbollist[i]);
        size_t begin = symbol.find('(');
        size_t end = symbol.find('+', begin);
        if (begin != std::string::npos && end != std::string::npos) {
            std::string functionName = symbol.substr(begin + 1, end - begin - 1);
            int status;
            char* demangledName = abi::__cxa_demangle(functionName.c_str(), nullptr, nullptr, &status);
            if (status == 0) {
                oss << "  " << demangledName << std::endl;
            } else {
                oss << "  " << functionName << std::endl;
            }
            free(demangledName);
        } else {
            oss << "  " << symbol << std::endl;
        }
    }
    free(symbollist);
    return oss.str();
}
