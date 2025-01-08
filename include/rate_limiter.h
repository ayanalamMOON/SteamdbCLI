#pragma once
#include <chrono>
#include <thread>

class RateLimiter {
public:
    RateLimiter(int requestsPerMinute = 30);
    void waitForNext();
    
private:
    std::chrono::steady_clock::time_point lastRequest;
    std::chrono::milliseconds delay;
};
