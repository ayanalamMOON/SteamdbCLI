#pragma once
#include <string>
#include <vector>

struct GameData {
    std::string name;
    std::string appId;
    std::string currentPrice;
    std::string lowestPrice;
    std::string metacritic;
    std::string releaseDate;
    std::vector<std::string> tags;
    std::string description;
};
