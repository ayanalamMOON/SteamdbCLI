#include <iostream>
#include <string>
#include "scraper.h"

int main() {
    std::cout << "Welcome to Steamdb CLI!" << std::endl;
    std::cout << "Type the name of a game to search for its information." << std::endl;

    while (true) {
        std::cout << "\nEnter game name (or type 'exit' to quit): ";
        std::string gameName;
        std::getline(std::cin, gameName);

        if (gameName == "exit") {
            break;
        }

        try {
            std::string gameData = Scraper::fetchGameData(gameName);
            std::cout << gameData << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    return 0;
}
