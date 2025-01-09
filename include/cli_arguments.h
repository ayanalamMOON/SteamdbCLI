#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class CliArguments {
public:
    // Show the help message
    static void showHelp();
    
    // Parse the game name from command line arguments
    static std::string parseGameName(int argc, char* argv[]);
    
    // Save the search history to a file
    static void saveSearchHistory(const std::vector<std::string>& searchHistory, const std::string& filename);
    
    // Load the search history from a file
    static std::vector<std::string> loadSearchHistory(const std::string& filename);

    // Display the user's Steam library
    static void displaySteamLibrary();

    // Display the user's wishlist
    static void displayWishlist();

    // Recommend games based on search history
    static void recommendGames(const std::vector<std::string>& searchHistory);
    
private:
    static const char* USAGE;
};

const char* CliArguments::USAGE = "Usage: steamdb_cli [options] <game_name>\n"
                                  "Options:\n"
                                  "  -h, --help    Show this help message\n";

// Display the usage message
void CliArguments::showHelp() {
    std::cout << USAGE;
}

// Parse the game name from command line arguments
std::string CliArguments::parseGameName(int argc, char* argv[]) {
    if (argc < 2) {
        showHelp();
        return "";
    }

    std::string gameName;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help") {
            showHelp();
            return "";
        } else {
            if (!gameName.empty()) {
                gameName += " ";
            }
            gameName += argv[i];
        }
    }

    return gameName;
}

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
