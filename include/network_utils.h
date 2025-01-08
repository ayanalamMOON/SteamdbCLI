#pragma once
#include <string>
#include <stdexcept>

class NetworkError : public std::runtime_error {
public:
    explicit NetworkError(const std::string& message) : std::runtime_error(message) {}
};

namespace NetworkUtils {
    bool checkInternetConnection();
    std::string urlEncode(const std::string& str);
    std::string constructUrl(const std::string& baseUrl, const std::string& query);
}
