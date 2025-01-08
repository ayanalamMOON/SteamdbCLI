#include "rate_limiter.h"

// Constructor to initialize the rate limiter with a specific rate
RateLimiter::RateLimiter(int requestsPerMinute) {
    delay = std::chrono::milliseconds(60000 / requestsPerMinute);
    lastRequest = std::chrono::steady_clock::now();
}

// Wait for the next allowed request time
void RateLimiter::waitForNext() {
    auto now = std::chrono::steady_clock::now();
    auto timeSinceLastRequest = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastRequest);
    if (timeSinceLastRequest < delay) {
        std::this_thread::sleep_for(delay - timeSinceLastRequest);
    }
    lastRequest = std::chrono::steady_clock::now();
}
