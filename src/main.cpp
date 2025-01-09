#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "scraper.h"
#include "logger.h"
#include "game_cache.h"

// Large scale ASCII header title
void printHeader() {
    std::cout << R"(
  _____ _                                 _     _____  _      _____ 
 / ____| |                               | |   |  __ \| |    |_   _|
| (___ | |__   ___  _ __ ___   __ _ _ __ | |_  | |  | | |      | |  
 \___ \| '_ \ / _ \| '_ ` _ \ / _` | '_ \| __| | |  | | |      | |  
 ____) | | | | (_) | | | | | | (_| | | | | |_  | |__| | |____ _| |_ 
|_____/|_| |_|\___/|_| |_| |_|\__,_|_| |_|\__| |_____/|______|_____|
    )" << std::endl;
}

// Function to display the current time
void displayCurrentTime() {
    std::time_t now = std::time(nullptr);
    std::cout << "Current Time: " << std::ctime(&now);
}

// Function to display a random game quote
void displayRandomQuote() {
    std::vector<std::string> quotes = {
        "The right man in the wrong place can make all the difference in the world. - Half-Life 2",
        "War. War never changes. - Fallout",
        "It's dangerous to go alone! Take this. - The Legend of Zelda",
        "Do a barrel roll! - Star Fox 64",
        "The cake is a lie. - Portal"
    };
    std::srand(std::time(nullptr));
    int randomIndex = std::rand() % quotes.size();
    std::cout << "Random Game Quote: " << quotes[randomIndex] << std::endl;
}

// Main function to run the Steamdb CLI program
int main() {
    Logger logger;
    logger.init("steamdb_cli.log");

    GameCache gameCache;
    std::vector<std::string> searchHistory;

    printHeader();
    displayCurrentTime();
    displayRandomQuote();

    std::cout << "Welcome to Steamdb CLI!" << std::endl;
    std::cout << "Type the name of a game to search for its information." << std::endl;

    while (true) {
        std::cout << "\nEnter game name (or type 'exit' to quit): ";
        std::string gameName;
        std::getline(std::cin, gameName);

        if (gameName == "exit") {
            break;
        }

        searchHistory.push_back(gameName);

        try {
            if (gameCache.hasGame(gameName)) {
                GameData cachedData = gameCache.getGame(gameName);
                std::cout << "Cached Data: " << cachedData.name << std::endl;
                std::cout << "App ID: " << cachedData.appId << std::endl;
                std::cout << "Current Price: " << cachedData.currentPrice << std::endl;
                std::cout << "Lowest Price: " << cachedData.lowestPrice << std::endl;
                std::cout << "Metacritic Score: " << cachedData.metacritic << std::endl;
                std::cout << "Release Date: " << cachedData.releaseDate << std::endl;
                std::cout << "Tags: ";
                for (const auto& tag : cachedData.tags) {
                    std::cout << tag << " ";
                }
                std::cout << std::endl;
                std::cout << "Description: " << cachedData.description << std::endl;
                logger.info("Fetched cached data for game: " + gameName);
            } else {
                Scraper scraper;
                GameData gameData = scraper.searchGame(gameName);
                gameCache.addGame(gameName, gameData);
                std::cout << "Fetched Data: " << gameData.name << std::endl;
                std::cout << "App ID: " << gameData.appId << std::endl;
                std::cout << "Current Price: " << gameData.currentPrice << std::endl;
                std::cout << "Lowest Price: " << gameData.lowestPrice << std::endl;
                std::cout << "Metacritic Score: " << gameData.metacritic << std::endl;
                std::cout << "Release Date: " << gameData.releaseDate << std::endl;
                std::cout << "Tags: ";
                for (const auto& tag : gameData.tags) {
                    std::cout << tag << " ";
                }
                std::cout << std::endl;
                std::cout << "Description: " << gameData.description << std::endl;
                logger.info("Fetched data for game: " + gameName);
            }
        } catch (const NetworkError& e) {
            std::cerr << "Network Error: " << e.what() << std::endl;
            std::cerr << "Please check your internet connection and try again." << std::endl;
            logger.error("Network error while fetching data for game: " + gameName, __FUNCTION__, __FILE__, __LINE__);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            std::cerr << "An unexpected error occurred. Please try again later." << std::endl;
            logger.error("Error while fetching data for game: " + gameName, __FUNCTION__, __FILE__, __LINE__);
        }
    }

    // Display search history
    std::cout << "\nSearch History:" << std::endl;
    for (const auto& search : searchHistory) {
        std::cout << search << std::endl;
    }

    return 0;
}
