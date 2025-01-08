#pragma once
#include <string>
#include <vector>
#include <iostream>

class CliArguments {
public:
    // Show the help message
    static void showHelp();
    
    // Parse the game name from command line arguments
    static std::string parseGameName(int argc, char* argv[]);
    
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
