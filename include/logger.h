#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>

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

    // Log a retry attempt for a failed network request
    void logRetryAttempt(const std::string& url, int attemptNumber);
    
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
