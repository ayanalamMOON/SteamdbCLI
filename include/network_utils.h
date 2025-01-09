#pragma once
#include <string>
#include <stdexcept>

// Represents an error that occurs during network operations
class NetworkError : public std::runtime_error {
public:
    explicit NetworkError(const std::string& message) : std::runtime_error(message) {}
};

namespace NetworkUtils {
    // Check if there is an active internet connection
    bool checkInternetConnection();
    
    // Encode a URL string to make it safe for use in a URL
    std::string urlEncode(const std::string& str);
    
    // Construct a URL by combining a base URL and a query string
    std::string constructUrl(const std::string& baseUrl, const std::string& query);

    // Retry failed network requests with exponential backoff
    std::string fetchPageWithRetry(const std::string& url, int maxRetries = 3, int retryDelay = 2000);
}
