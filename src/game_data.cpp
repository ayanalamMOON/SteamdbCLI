#include "game_data.h"

// Constructor to initialize the game data
GameData::GameData(const std::string& name, const std::string& appId, const std::string& currentPrice,
                   const std::string& lowestPrice, const std::string& metacritic, const std::string& releaseDate,
                   const std::vector<std::string>& tags, const std::string& description, const std::string& reviewScore)
    : name(name), appId(appId), currentPrice(currentPrice), lowestPrice(lowestPrice), metacritic(metacritic),
      releaseDate(releaseDate), tags(tags), description(description), reviewScore(reviewScore) {}

// Get the name of the game
std::string GameData::getName() const {
    return name;
}

// Get the application ID of the game
std::string GameData::getAppId() const {
    return appId;
}

// Get the current price of the game
std::string GameData::getCurrentPrice() const {
    return currentPrice;
}

// Get the lowest price of the game
std::string GameData::getLowestPrice() const {
    return lowestPrice;
}

// Get the Metacritic score of the game
std::string GameData::getMetacritic() const {
    return metacritic;
}

// Get the release date of the game
std::string GameData::getReleaseDate() const {
    return releaseDate;
}

// Get the tags associated with the game
std::vector<std::string> GameData::getTags() const {
    return tags;
}

// Get the description of the game
std::string GameData::getDescription() const {
    return description;
}

// Get the review score of the game
std::string GameData::getReviewScore() const {
    return reviewScore;
}
