#include <stdexcept>

class NetworkError : public std::runtime_error {
public:
    // Represents an error that occurs during network operations
    explicit NetworkError(const std::string& message) : std::runtime_error(message) {}
};
