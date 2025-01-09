#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "scraper.h"
#include "logger.h"
#include "game_cache.h"
#include "config.h"
#include <iomanip>

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

// Function to apply user configurations
void applyUserConfigurations(Config& config) {
    std::string colorOutput = config.get("color_output");
    if (colorOutput == "true") {
        std::cout << "\033[1;32m"; // Set text color to green
    }
}

// Function to reset text color
void resetTextColor() {
    std::cout << "\033[0m"; // Reset text color
}

// Function to display game information in a tabular format
void displayGameInfo(const GameData& gameData) {
    std::cout << std::left << std::setw(20) << "Name" << ": " << gameData.name << std::endl;
    std::cout << std::left << std::setw(20) << "App ID" << ": " << gameData.appId << std::endl;
    std::cout << std::left << std::setw(20) << "Current Price" << ": " << gameData.currentPrice << std::endl;
    std::cout << std::left << std::setw(20) << "Lowest Price" << ": " << gameData.lowestPrice << std::endl;
    std::cout << std::left << std::setw(20) << "Metacritic Score" << ": " << gameData.metacritic << std::endl;
    std::cout << std::left << std::setw(20) << "Release Date" << ": " << gameData.releaseDate << std::endl;
    std::cout << std::left << std::setw(20) << "Tags" << ": ";
    for (const auto& tag : gameData.tags) {
        std::cout << tag << " ";
    }
    std::cout << std::endl;
    std::cout << std::left << std::setw(20) << "Description" << ": " << gameData.description << std::endl;
    std::cout << std::left << std::setw(20) << "Review Score" << ": " << gameData.reviewScore << std::endl;
}

// Main function to run the Steamdb CLI program
int main() {
    Logger logger;
    logger.init("steamdb_cli.log");

    GameCache gameCache;
    std::vector<std::string> searchHistory;

    Config config;
    config.load("config.txt");
    applyUserConfigurations(config);

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
                displayGameInfo(cachedData);
                logger.info("Fetched cached data for game: " + gameName);
            } else {
                Scraper scraper;
                GameData gameData = scraper.searchGame(gameName);
                gameCache.addGame(gameName, gameData);
                displayGameInfo(gameData);
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

    resetTextColor();

    return 0;
}
