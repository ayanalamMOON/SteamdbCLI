#include <stdexcept>

class NetworkError : public std::runtime_error {
public:
    explicit NetworkError(const std::string& message) : std::runtime_error(message) {}
};
