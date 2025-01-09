#include "logger.h"

// Log a retry attempt for a failed network request
void Logger::logRetryAttempt(const std::string& url, int attemptNumber) {
    std::ostringstream oss;
    oss << "Retry attempt " << attemptNumber << " for URL: " << url;
    log("RETRY", oss.str());
}
