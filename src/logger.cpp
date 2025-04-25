#include "logger.h"

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

// Log a retry attempt for a failed network request
void Logger::logRetryAttempt(const std::string& url, int attemptNumber) {
    std::ostringstream oss;
    oss << "Retry attempt " << attemptNumber << " for URL: " << url;
    log("RETRY", oss.str());
}

// Log a message with a specific level
void Logger::log(const std::string& level, const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    if (logFile.is_open()) {
        logFile << "[" << getCurrentTimestamp() << "] [" << level << "] " << message << std::endl;
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

// Get the stack trace - Windows compatible version
std::string Logger::getStackTrace() {
    // This is a simplified implementation for Windows
    return "  Stack trace not available on this platform";
}
