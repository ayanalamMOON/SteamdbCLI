#pragma once
#include <string>
#include <vector>

// Structure to hold game data
struct GameData {
    std::string name; // Name of the game
    std::string appId; // Application ID of the game
    std::string currentPrice; // Current price of the game
    std::string lowestPrice; // Lowest price of the game
    std::string metacritic; // Metacritic score of the game
    std::string releaseDate; // Release date of the game
    std::vector<std::string> tags; // Tags associated with the game
    std::string description; // Description of the game
};
