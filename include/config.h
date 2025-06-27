#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

class Config
{
public:
    // Load the configuration from a file
    void load(const std::string &filename);

    // Get a configuration value by key
    std::string get(const std::string &key) const;

    // Set a configuration value by key
    void set(const std::string &key, const std::string &value);

    // Save the configuration to a file
    void save(const std::string &filename) const;

    // Steam API specific methods
    std::string getSteamApiKey() const;
    void setSteamApiKey(const std::string &apiKey);
    bool hasSteamApiKey() const;

    // Static method to get the default config instance
    static Config &getInstance();

private:
    std::unordered_map<std::string, std::string> configMap;
    static Config instance;
};
