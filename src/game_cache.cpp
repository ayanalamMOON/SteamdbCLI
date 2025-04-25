#include "game_cache.h"

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