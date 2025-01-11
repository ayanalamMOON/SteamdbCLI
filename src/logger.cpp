#include "logger.h"

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
