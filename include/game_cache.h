#pragma once
#include "game_data.h"
#include <unordered_map>
#include <string>
#include <mutex>

class GameCache {
public:
    bool hasGame(const std::string& gameName);
    void addGame(const std::string& gameName, const GameData& data);
    GameData getGame(const std::string& gameName);
    void clear();

private:
    std::unordered_map<std::string, GameData> cache;
    std::mutex cacheMutex;
};
