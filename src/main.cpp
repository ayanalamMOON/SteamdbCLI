#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <fstream>
#include "scraper.h"
#include "logger.h"
#include "game_cache.h"
#include "config.h"
#include "steam_api_helper.h"
#include <iomanip>
#include "cli_arguments.h"

// Large scale ASCII header title
void printHeader()
{
    std::cout << R"(
 ____  _                      ____  ____     ____ _     ___
/ ___|| |_ ___  __ _ _ __ __|  _ \| __ )   / ___| |   |_ _|
\___ \| __/ _ \| `_ ` _ \/ _` | | | |  _ \  | |   | |    | |
 ___) | || (_) | | | | | | (_| | |_| | |_) | | |___| |___ | |
|____/ \__\___/|_| |_| |_|\__,_|____/|____/  \____|_____|___|
    )" << std::endl;
    std::cout << "           Your terminal is now a gaming database" << std::endl;
    std::cout << "           ------------------------------------" << std::endl;
}

// Function to display the current time
void displayCurrentTime()
{
    std::time_t now = std::time(nullptr);
    std::cout << "Current Time: " << std::ctime(&now);
}

// Function to display a random game quote
void displayRandomQuote()
{
    std::vector<std::string> quotes = {
        "The right man in the wrong place can make all the difference in the world. - Half-Life 2",
        "War. War never changes. - Fallout",
        "It's dangerous to go alone! Take this. - The Legend of Zelda",
        "Do a barrel roll! - Star Fox 64",
        "The cake is a lie. - Portal"};
    std::srand(std::time(nullptr));
    int randomIndex = std::rand() % quotes.size();
    std::cout << "Random Game Quote: " << quotes[randomIndex] << std::endl;
}

// Function to apply user configurations
void applyUserConfigurations(Config &config)
{
    std::string colorOutput = config.get("color_output");
    if (colorOutput == "true")
    {
        std::cout << "\033[1;32m"; // Set text color to green
    }
}

// Function to reset text color
void resetTextColor()
{
    std::cout << "\033[0m"; // Reset text color
}

// Function to display game information in a tabular format
void displayGameInfo(const GameData &gameData)
{
    std::cout << std::left << std::setw(20) << "Name" << ": " << gameData.name << std::endl;
    std::cout << std::left << std::setw(20) << "App ID" << ": " << gameData.appId << std::endl;
    std::cout << std::left << std::setw(20) << "Current Price" << ": " << gameData.currentPrice << std::endl;
    std::cout << std::left << std::setw(20) << "Lowest Price" << ": " << gameData.lowestPrice << std::endl;
    std::cout << std::left << std::setw(20) << "Metacritic Score" << ": " << gameData.metacritic << std::endl;
    std::cout << std::left << std::setw(20) << "Release Date" << ": " << gameData.releaseDate << std::endl;
    std::cout << std::left << std::setw(20) << "Tags" << ": ";
    for (const auto &tag : gameData.tags)
    {
        std::cout << tag << " ";
    }
    std::cout << std::endl;
    std::cout << std::left << std::setw(20) << "Description" << ": " << gameData.description << std::endl;
    std::cout << std::left << std::setw(20) << "Review Score" << ": " << gameData.reviewScore << std::endl;
}

// Function to display a progress bar
void displayProgressBar(int progress, int total)
{
    int barWidth = 50;
    float progressRatio = static_cast<float>(progress) / total;
    int pos = static_cast<int>(barWidth * progressRatio);

    std::cout << "[";
    for (int i = 0; i < barWidth; ++i)
    {
        if (i < pos)
            std::cout << "=";
        else if (i == pos)
            std::cout << ">";
        else
            std::cout << " ";
    }
    std::cout << "] " << int(progressRatio * 100.0) << " %\r";
    std::cout.flush();
}

// Function to display Steam sale information
void displaySaleInfo(const SteamSaleInfo &saleInfo, int index)
{
    std::cout << "\n--- Game " << index << " ---" << std::endl;
    std::cout << "Name: " << saleInfo.name << std::endl;
    std::cout << "App ID: " << saleInfo.appId << std::endl;

    if (!saleInfo.currentPrice.empty())
    {
        if (!saleInfo.discountPercent.empty() && saleInfo.discountPercent != "0")
        {
            // It's on sale
            std::cout << "🔥 ON SALE: " << saleInfo.currentPrice;
            if (!saleInfo.currency.empty())
            {
                std::cout << " " << saleInfo.currency;
            }
            if (!saleInfo.originalPrice.empty())
            {
                std::cout << " (was " << saleInfo.originalPrice;
                if (!saleInfo.currency.empty())
                {
                    std::cout << " " << saleInfo.currency;
                }
                std::cout << ", " << saleInfo.discountPercent << "% off)";
            }
            std::cout << std::endl;
        }
        else
        {
            // Regular price
            std::cout << "Price: " << saleInfo.currentPrice;
            if (!saleInfo.currency.empty())
            {
                std::cout << " " << saleInfo.currency;
            }
            std::cout << std::endl;
        }
    }
    else
    {
        std::cout << "Price: Information not available" << std::endl;
    }

    if (!saleInfo.saleEndDate.empty())
    {
        std::cout << "Sale ends: " << saleInfo.saleEndDate << std::endl;
    }

    if (saleInfo.isHighlighted)
    {
        std::cout << "🌟 Great Deal!" << std::endl;
    }

    std::cout << "-------------------" << std::endl;
}

// Function to display Steam game information
void displaySteamGameInfo(const SteamGameInfo &gameInfo)
{
    std::cout << "\n=== Steam Game Information ===" << std::endl;
    std::cout << "Name: " << gameInfo.name << std::endl;
    std::cout << "App ID: " << gameInfo.appId << std::endl;

    if (!gameInfo.developer.empty())
    {
        std::cout << "Developer: " << gameInfo.developer << std::endl;
    }

    if (!gameInfo.publisher.empty())
    {
        std::cout << "Publisher: " << gameInfo.publisher << std::endl;
    }

    if (!gameInfo.releaseDate.empty())
    {
        std::cout << "Release Date: " << gameInfo.releaseDate << std::endl;
    }

    if (gameInfo.isFree)
    {
        std::cout << "Price: Free to Play" << std::endl;
    }
    else if (!gameInfo.price.empty())
    {
        std::cout << "Price: " << gameInfo.price << std::endl;
    }

    if (!gameInfo.description.empty())
    {
        std::cout << "Description: " << gameInfo.description << std::endl;
    }

    std::cout << "==============================\n"
              << std::endl;
}

// Main function to run the Steamdb CLI program
int main(int argc, char *argv[])
{
    Logger logger;
    logger.init("steamdb_cli.log");

    GameCache gameCache;
    std::vector<std::string> searchHistory;

    Config &config = Config::getInstance();

    // Try to load config from multiple locations
    bool configLoaded = false;
    std::vector<std::string> configPaths = {"../config.txt", "config.txt", "../../config.txt"};

    for (const std::string &path : configPaths)
    {
        std::ifstream testFile(path);
        if (testFile.good())
        {
            testFile.close();
            config.load(path);
            configLoaded = true;
            std::cout << "Configuration loaded from: " << path << std::endl;

            // Debug: Check if Steam API key was actually loaded
            std::string apiKey = config.get("STEAM_API_KEY");
            if (!apiKey.empty())
            {
                std::cout << "Steam API key found in configuration." << std::endl;
            }
            else
            {
                std::cout << "Warning: Steam API key not found in " << path << std::endl;
            }
            break;
        }
    }

    if (!configLoaded)
    {
        std::cout << "Warning: config.txt not found. Please ensure config.txt is in the same directory as the executable." << std::endl;
    }

    applyUserConfigurations(config);

    // Initialize Steam API helper
    SteamApiHelper steamApi;
    bool steamApiAvailable = steamApi.initialize();
    if (!steamApiAvailable)
    {
        std::cout << "Warning: Steam API not available. Some features may be limited." << std::endl;
        logger.warning("Steam API initialization failed");
    }

    printHeader();
    displayCurrentTime();
    displayRandomQuote();

    std::cout << "Welcome to Steamdb CLI!" << std::endl;
    std::cout << "Choose an option:" << std::endl;
    std::cout << "1. Search for a game by name or App ID" << std::endl;
    std::cout << "2. View current Steam sales" << std::endl;
    std::cout << "3. View featured sales" << std::endl;
    std::cout << "4. View special offers" << std::endl;
    std::cout << "Type 'exit' to quit" << std::endl;

    while (true)
    {
        std::cout << "\nEnter your choice (1-4) or game name/App ID: ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "exit")
        {
            break;
        }

        // Handle menu options
        if (input == "1")
        {
            std::cout << "Enter game name or App ID: ";
            std::getline(std::cin, input);
            if (input == "exit")
                break;
        }
        else if (input == "2")
        {
            // Show current sales
            if (steamApiAvailable)
            {
                try
                {
                    std::cout << "\nFetching current Steam sales..." << std::endl;
                    std::vector<SteamSaleInfo> sales = steamApi.getCurrentSales(20);

                    if (!sales.empty())
                    {
                        std::cout << "\n=== Current Steam Sales ===" << std::endl;
                        for (size_t i = 0; i < sales.size(); ++i)
                        {
                            displaySaleInfo(sales[i], i + 1);
                        }
                    }
                    else
                    {
                        std::cout << "No current sales found." << std::endl;
                    }
                }
                catch (const std::exception &e)
                {
                    std::cout << "Error fetching sales: " << e.what() << std::endl;
                    logger.error("Error fetching current sales", __FUNCTION__, __FILE__, __LINE__);
                }
            }
            else
            {
                std::cout << "Steam API not available. Cannot fetch sales information." << std::endl;
            }
            continue;
        }
        else if (input == "3")
        {
            // Show featured sales
            if (steamApiAvailable)
            {
                try
                {
                    std::cout << "\nFetching featured Steam sales..." << std::endl;
                    std::vector<SteamSaleInfo> sales = steamApi.getFeaturedSales();

                    if (!sales.empty())
                    {
                        std::cout << "\n=== Featured Steam Sales ===" << std::endl;
                        for (size_t i = 0; i < sales.size(); ++i)
                        {
                            displaySaleInfo(sales[i], i + 1);
                        }
                    }
                    else
                    {
                        std::cout << "No featured sales found." << std::endl;
                    }
                }
                catch (const std::exception &e)
                {
                    std::cout << "Error fetching featured sales: " << e.what() << std::endl;
                    logger.error("Error fetching featured sales", __FUNCTION__, __FILE__, __LINE__);
                }
            }
            else
            {
                std::cout << "Steam API not available. Cannot fetch sales information." << std::endl;
            }
            continue;
        }
        else if (input == "4")
        {
            // Show special offers
            if (steamApiAvailable)
            {
                try
                {
                    std::cout << "\nFetching special offers..." << std::endl;
                    std::vector<SteamSaleInfo> sales = steamApi.getSpecialOffers();

                    if (!sales.empty())
                    {
                        std::cout << "\n=== Special Offers ===" << std::endl;
                        for (size_t i = 0; i < sales.size(); ++i)
                        {
                            displaySaleInfo(sales[i], i + 1);
                        }
                    }
                    else
                    {
                        std::cout << "No special offers found." << std::endl;
                    }
                }
                catch (const std::exception &e)
                {
                    std::cout << "Error fetching special offers: " << e.what() << std::endl;
                    logger.error("Error fetching special offers", __FUNCTION__, __FILE__, __LINE__);
                }
            }
            else
            {
                std::cout << "Steam API not available. Cannot fetch sales information." << std::endl;
            }
            continue;
        }

        // If not a menu option, treat as game search
        std::string gameName = input;

        searchHistory.push_back(gameName);

        try
        {
            if (gameCache.hasGame(gameName))
            {
                GameData cachedData = gameCache.getGame(gameName);
                displayGameInfo(cachedData);
                logger.info("Fetched cached data for game: " + gameName);
            }
            else
            {
                bool foundWithSteamApi = false;

                // Try Steam API first if available
                if (steamApiAvailable)
                {
                    try
                    {
                        // Check if input looks like a Steam App ID (numeric)
                        bool isAppId = true;
                        for (char c : gameName)
                        {
                            if (!std::isdigit(c))
                            {
                                isAppId = false;
                                break;
                            }
                        }

                        if (isAppId && gameName.length() >= 1)
                        {
                            // Direct App ID lookup
                            std::cout << "Fetching Steam API data for App ID: " << gameName << std::endl;
                            SteamGameInfo steamGame = steamApi.getGameInfo(gameName);

                            if (!steamGame.name.empty())
                            {
                                displaySteamGameInfo(steamGame);
                                foundWithSteamApi = true;
                                logger.info("Fetched Steam API data for App ID: " + gameName);
                            }
                        }
                        else
                        {
                            // Game name search using Steam API
                            std::cout << "Searching Steam for: " << gameName << std::endl;
                            std::vector<SteamGameInfo> searchResults = steamApi.searchGames(gameName);

                            if (!searchResults.empty())
                            {
                                std::cout << "Found " << searchResults.size() << " result(s):" << std::endl;

                                for (size_t i = 0; i < searchResults.size(); ++i)
                                {
                                    std::cout << "\n--- Result " << (i + 1) << " ---" << std::endl;
                                    displaySteamGameInfo(searchResults[i]);
                                }

                                foundWithSteamApi = true;
                                logger.info("Found Steam API results for: " + gameName);
                            }
                            else
                            {
                                std::cout << "No games found in Steam database for: " << gameName << std::endl;
                                std::cout << "Tip: Try using the exact Steam App ID for more accurate results." << std::endl;
                            }
                        }
                    }
                    catch (const std::exception &e)
                    {
                        std::cout << "Steam API search failed, falling back to web scraping..." << std::endl;
                        logger.warning("Steam API search failed for: " + gameName + " - " + e.what());
                    }
                }

                // Fall back to web scraping if Steam API didn't work
                if (!foundWithSteamApi)
                {
                    Scraper scraper;
                    std::cout << "Fetching data for game: " << gameName << std::endl;
                    for (int i = 0; i <= 100; ++i)
                    {
                        displayProgressBar(i, 100);
                        std::this_thread::sleep_for(std::chrono::milliseconds(20));
                    }
                    std::cout << std::endl;
                    GameData gameData = scraper.searchGame(gameName);
                    gameCache.addGame(gameName, gameData);
                    displayGameInfo(gameData);
                    logger.info("Fetched data for game: " + gameName);
                }
            }
        }
        catch (const NetworkError &e)
        {
            std::cerr << "\033[1;31mNetwork Error: " << e.what() << "\033[0m" << std::endl;
            std::cerr << "Please check your internet connection and try again." << std::endl;
            logger.error("Network error while fetching data for game: " + gameName, __FUNCTION__, __FILE__, __LINE__);
        }
        catch (const std::exception &e)
        {
            std::cerr << "\033[1;31mError: " << e.what() << "\033[0m" << std::endl;
            std::cerr << "An unexpected error occurred. Please try again later." << std::endl;
            logger.error("Error while fetching data for game: " + gameName, __FUNCTION__, __FILE__, __LINE__);
        }
    }

    // Display search history
    std::cout << "\nSearch History:" << std::endl;
    for (const auto &search : searchHistory)
    {
        std::cout << search << std::endl;
    }

    resetTextColor();

    return 0;
}
