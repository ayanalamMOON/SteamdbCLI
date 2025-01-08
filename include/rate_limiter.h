#pragma once
#include <chrono>
#include <thread>

// RateLimiter class to control the rate of requests
class RateLimiter {
public:
    // Constructor to initialize the rate limiter with a specific rate
    RateLimiter(int requestsPerMinute = 30);
    
    // Wait for the next allowed request time
    void waitForNext();
    
private:
    std::chrono::steady_clock::time_point lastRequest; // Time point of the last request
    std::chrono::milliseconds delay; // Delay between requests
};
