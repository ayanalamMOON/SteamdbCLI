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

bool GameCache::hasGame(const std::string& gameName) {
    std::lock_guard<std::mutex> lock(cacheMutex);
    return cache.find(gameName) != cache.end();
}

void GameCache::addGame(const std::string& gameName, const GameData& data) {
    std::lock_guard<std::mutex> lock(cacheMutex);
    cache[gameName] = data;
}

GameData GameCache::getGame(const std::string& gameName) {
    std::lock_guard<std::mutex> lock(cacheMutex);
    return cache.at(gameName);
}

void GameCache::clear() {
    std::lock_guard<std::mutex> lock(cacheMutex);
    cache.clear();
}
