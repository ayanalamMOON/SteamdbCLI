#pragma once
#include "game_data.h"
#include <unordered_map>
#include <string>
#include <mutex>

class GameCache {
public:
    // Check if the cache contains data for the given game
    bool hasGame(const std::string& gameName);

    // Add game data to the cache
    void addGame(const std::string& gameName, const GameData& data);

    // Retrieve game data from the cache
    GameData getGame(const std::string& gameName);

    // Clear the cache
    void clear();

private:
    std::unordered_map<std::string, GameData> cache;
    std::mutex cacheMutex;
};
