#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class CliArguments {
public:
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

    // Parse command-line arguments for options and game names
    static void parseArguments(int argc, char* argv[], std::string& gameName, std::vector<std::string>& options);

    // Display help message and usage information
    static void displayHelpMessage();

    // Prompt the user for a game name
    static std::string getGameNameFromUser();
    
    // Handle runtime game name input
    static std::string handleRuntimeGameNameInput();
    
private:
    static const char* USAGE;
};

const char* CliArguments::USAGE = "Usage: steamdb_cli [options] <game_name>\n"
                                  "Options:\n"
                                  "  -h, --help    Show this help message\n";

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

// Parse command-line arguments for options and game names
void CliArguments::parseArguments(int argc, char* argv[], std::string& gameName, std::vector<std::string>& options) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            displayHelpMessage();
            exit(0);
        } else if (arg[0] == '-') {
            options.push_back(arg);
        } else {
            if (!gameName.empty()) {
                gameName += " ";
            }
            gameName += arg;
        }
    }
}

// Display help message and usage information
void CliArguments::displayHelpMessage() {
    std::cout << USAGE;
}

// Display the user's Steam library
void CliArguments::displaySteamLibrary() {
    // Placeholder implementation
    std::cout << "Displaying user's Steam library..." << std::endl;
}

// Display the user's wishlist
void CliArguments::displayWishlist() {
    // Placeholder implementation
    std::cout << "Displaying user's wishlist..." << std::endl;
}

// Recommend games based on search history
void CliArguments::recommendGames(const std::vector<std::string>& searchHistory) {
    // Placeholder implementation
    std::cout << "Recommending games based on search history..." << std::endl;
}

// Prompt the user for a game name
std::string CliArguments::getGameNameFromUser() {
    std::string gameName;
    std::cout << "Enter game name: ";
    std::getline(std::cin, gameName);
    return gameName;
}

// Handle runtime game name input
std::string CliArguments::handleRuntimeGameNameInput() {
    std::string gameName;
    std::cout << "Enter game name: ";
    std::getline(std::cin, gameName);
    return gameName;
}
