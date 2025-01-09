#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "scraper.h"
#include "game_cache.h"

// Function to display the large scale header design
void displayHeader(int searchCount) {
    // ASCII art for the header design
    std::string header = R"(
  _____ _                                 ____ _     ___ 
 / ____| |                               |  _ \ |   / _ \
| (___ | |_ ___  __ _ _ __ ___   ___ _ __| |_) | | _| | | |
 \___ \| __/ _ \/ _` | '_ ` _ \ / _ \ '__|  _ <| |/ / | | |
 ____) | ||  __/ (_| | | | | | |  __/ |  | |_) |   <| |_| |
|_____/ \__\___|\__,_|_| |_| |_|\___|_|  |____/|_|\_\\___/ 
    )";

    // ANSI escape codes for color and styling
    std::string colorHeader = "\033[1;34m" + header + "\033[0m"; // Blue color for header
    std::string boldUnderline = "\033[1;4m"; // Bold and underline
    std::string reset = "\033[0m"; // Reset styling

    // Get the current date and time
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    char dateTime[100];
    std::strftime(dateTime, sizeof(dateTime), "%Y-%m-%d %H:%M:%S", localTime);

    // Random game-related quotes or fun facts
    std::vector<std::string> quotes = {
        "Did you know? The first video game ever created was 'Tennis for Two' in 1958.",
        "Fun fact: The best-selling video game of all time is 'Minecraft'.",
        "Did you know? The longest gaming session lasted over 35 hours.",
        "Fun fact: The first gaming console was the Magnavox Odyssey, released in 1972.",
        "Did you know? The most expensive video game ever developed was 'Grand Theft Auto V'."
    };
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::string randomQuote = quotes[std::rand() % quotes.size()];

    // Display the header with color, styling, date, time, quote, and search count
    std::cout << colorHeader << std::endl;
    std::cout << boldUnderline << "Welcome to Steamdb CLI!" << reset << std::endl;
    std::cout << "Current Date and Time: " << dateTime << std::endl;
    std::cout << "Random Quote: " << randomQuote << std::endl;
    std::cout << "Number of Games Searched: " << searchCount << std::endl;
    std::cout << "Type the name of a game to search for its information." << std::endl;
}

// Main function to run the Steamdb CLI program
int main() {
    GameCache gameCache;
    int searchCount = 0;

    displayHeader(searchCount);

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
                std::cout << "Developer: " << cachedData.developer << std::endl;
                std::cout << "Publisher: " << cachedData.publisher << std::endl;
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
                std::cout << "Developer: " << gameData.developer << std::endl;
                std::cout << "Publisher: " << gameData.publisher << std::endl;
            }
            searchCount++;
            displayHeader(searchCount);
        } catch (const NetworkError& e) {
            std::cerr << "Network Error: " << e.what() << std::endl;
            std::cerr << "Please check your internet connection and try again." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            std::cerr << "An unexpected error occurred. Please try again later." << std::endl;
        }
    }

    return 0;
}
