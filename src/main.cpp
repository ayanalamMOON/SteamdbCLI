#include <iostream>
#include <string>
#include "scraper.h"
#include "logger.h"
#include "game_cache.h"

// Main function to run the Steamdb CLI program
int main() {
    Logger logger;
    logger.init("steamdb_cli.log");

    GameCache gameCache;

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
            if (gameCache.hasGame(gameName)) {
                GameData cachedData = gameCache.getGame(gameName);
                std::cout << "Cached Data: " << cachedData.name << std::endl;
                logger.info("Fetched cached data for game: " + gameName);
            } else {
                Scraper scraper;
                GameData gameData = scraper.searchGame(gameName);
                gameCache.addGame(gameName, gameData);
                std::cout << "Fetched Data: " << gameData.name << std::endl;
                logger.info("Fetched data for game: " + gameName);
            }
        } catch (const NetworkError& e) {
            std::cerr << "Network Error: " << e.what() << std::endl;
            logger.error("Network error while fetching data for game: " + gameName);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            logger.error("Error while fetching data for game: " + gameName);
        }
    }

    return 0;
}
