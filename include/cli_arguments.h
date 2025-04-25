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

// Define the USAGE string here since it's a static const
const char* CliArguments::USAGE = "Usage: steamdb_cli [options] <game_name>\n"
                                  "Options:\n"
                                  "  -h, --help    Show this help message\n";
