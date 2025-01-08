#pragma once
#include "game_data.h"
#include "rate_limiter.h"
#include <string>
#include <memory>
#include "network_utils.h"

class Scraper {
public:
    Scraper();
    ~Scraper();
    GameData searchGame(const std::string& gameName);
    
private:
    std::string fetchPage(const std::string& url);
    GameData parseGameData(const std::string& html);
    std::unique_ptr<RateLimiter> rateLimiter;
};
