#pragma once
#include "game_data.h"
#include "rate_limiter.h"
#include <string>
#include <memory>
#include "network_utils.h"

// Scraper class to handle web scraping operations
class Scraper {
public:
    // Constructor to initialize the scraper
    Scraper();
    
    // Destructor to clean up resources
    ~Scraper();
    
    // Search for a game by name and return its data
    GameData searchGame(const std::string& gameName);
    
private:
    // Fetch the HTML content of a web page
    std::string fetchPage(const std::string& url);
    
    // Parse the HTML content to extract game data
    GameData parseGameData(const std::string& html);
    
    // Rate limiter to control the rate of requests
    std::unique_ptr<RateLimiter> rateLimiter;
};
