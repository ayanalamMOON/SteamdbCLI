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
