#include "cli_arguments.h"

// Save the search history to a file
void CliArguments::saveSearchHistory(const std::vector<std::string>& searchHistory, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error: Unable to open file for writing: " << filename << std::endl;
        return;
    }
    for (const auto& search : searchHistory) {
        outFile << search << std::endl;
    }
}

// Load the search history from a file
std::vector<std::string> CliArguments::loadSearchHistory(const std::string& filename) {
    std::vector<std::string> searchHistory;
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error: Unable to open file for reading: " << filename << std::endl;
        return searchHistory;
    }
    std::string line;
    while (std::getline(inFile, line)) {
        searchHistory.push_back(line);
    }
    return searchHistory;
}
