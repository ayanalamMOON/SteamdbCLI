#include "config.h"

// Static instance for singleton pattern
Config Config::instance;

// Load the configuration from a file
void Config::load(const std::string &filename)
{
    std::ifstream inFile(filename);
    if (!inFile)
    {
        std::cerr << "Error: Unable to open configuration file for reading: " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(inFile, line))
    {
        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos)
        {
            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);
            configMap[key] = value;
        }
    }
}

// Get a configuration value by key
std::string Config::get(const std::string &key) const
{
    auto it = configMap.find(key);
    if (it != configMap.end())
    {
        return it->second;
    }
    return "";
}

// Set a configuration value by key
void Config::set(const std::string &key, const std::string &value)
{
    configMap[key] = value;
}

// Save the configuration to a file
void Config::save(const std::string &filename) const
{
    std::ofstream outFile(filename);
    if (!outFile)
    {
        std::cerr << "Error: Unable to open configuration file for writing: " << filename << std::endl;
        return;
    }
    for (const auto &pair : configMap)
    {
        outFile << pair.first << "=" << pair.second << std::endl;
    }
}

// Steam API specific methods
std::string Config::getSteamApiKey() const
{
    return get("STEAM_API_KEY");
}

void Config::setSteamApiKey(const std::string &apiKey)
{
    set("STEAM_API_KEY", apiKey);
}

bool Config::hasSteamApiKey() const
{
    std::string apiKey = getSteamApiKey();
    return !apiKey.empty();
}

// Static method to get the default config instance
Config &Config::getInstance()
{
    return instance;
}
