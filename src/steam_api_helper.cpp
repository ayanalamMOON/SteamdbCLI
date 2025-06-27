#include "steam_api_helper.h"
#include "network_utils.h"
#include "config.h"
#include "logger.h"
#include <chrono>
#include <thread>
#include <algorithm>
#include <sstream>
#include <map>
#include <set>
#include <cctype>

bool SteamApiHelper::initialize()
{
    Config &config = Config::getInstance();

    if (!config.hasSteamApiKey())
    {
        std::cerr << "Error: Steam API key not found in configuration." << std::endl;
        std::cerr << "Please add your Steam API key to config.txt" << std::endl;
        return false;
    }

    std::string apiKey = config.getSteamApiKey();
    apiKeyValid = NetworkUtils::validateSteamApiKey(apiKey);

    if (!apiKeyValid)
    {
        std::cerr << "Error: Invalid Steam API key provided." << std::endl;
        std::cerr << "Please check your Steam API key in config.txt" << std::endl;
        return false;
    }

    lastApiCall = 0;
    callsInLastMinute = 0;

    std::cout << "Steam API initialized successfully." << std::endl;
    return true;
}

SteamGameInfo SteamApiHelper::getGameInfo(const std::string &appId)
{
    SteamGameInfo gameInfo;
    gameInfo.appId = appId;

    try
    {
        // Get basic app details from Steam Store API
        std::string storeParams = "appids=" + appId + "&cc=US&l=en";
        std::string storeResponse = makeApiCall("/appdetails", storeParams);

        // Parse the response (basic parsing - in a real implementation, you'd use a JSON library)
        gameInfo.name = parseJsonValue(storeResponse, "name");
        gameInfo.description = parseJsonValue(storeResponse, "short_description");
        gameInfo.developer = parseJsonValue(storeResponse, "developers");
        gameInfo.publisher = parseJsonValue(storeResponse, "publishers");
        gameInfo.releaseDate = parseJsonValue(storeResponse, "release_date");
        gameInfo.headerImage = parseJsonValue(storeResponse, "header_image");
        gameInfo.metacriticScore = parseJsonValue(storeResponse, "metacritic");

        // Check if the game is free
        std::string isFreeStr = parseJsonValue(storeResponse, "is_free");
        gameInfo.isFree = (isFreeStr == "true");

        if (!gameInfo.isFree)
        {
            std::string priceOverview = parseJsonValue(storeResponse, "price_overview");
            gameInfo.price = parseJsonValue(priceOverview, "final_formatted");
            gameInfo.originalPrice = parseJsonValue(priceOverview, "initial_formatted");
            gameInfo.discountPercent = parseJsonValue(priceOverview, "discount_percent");
            gameInfo.currency = parseJsonValue(priceOverview, "currency");

            // Check if on sale
            std::string discountPercent = parseJsonValue(priceOverview, "discount_percent");
            gameInfo.isOnSale = (!discountPercent.empty() && discountPercent != "0");
        }
        else
        {
            gameInfo.isOnSale = false;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error fetching game info for app ID " << appId << ": " << e.what() << std::endl;
    }

    return gameInfo;
}

std::vector<SteamGameInfo> SteamApiHelper::searchGames(const std::string &searchTerm)
{
    std::vector<SteamGameInfo> results;

    try
    {
        // Create a map of popular games for quick lookup
        // In a full implementation, you'd want to cache the full Steam app list
        std::map<std::string, std::string> popularGames = {
            {"god of war", "1593500"},
            {"cyberpunk 2077", "1091500"},
            {"cyberpunk", "1091500"},
            {"dota 2", "570"},
            {"dota", "570"},
            {"counter-strike 2", "730"},
            {"cs2", "730"},
            {"counter-strike", "730"},
            {"team fortress 2", "440"},
            {"tf2", "440"},
            {"half-life 2", "220"},
            {"portal", "400"},
            {"portal 2", "620"},
            {"left 4 dead 2", "550"},
            {"gta v", "271590"},
            {"grand theft auto v", "271590"},
            {"the witcher 3", "292030"},
            {"witcher 3", "292030"},
            {"skyrim", "72850"},
            {"elder scrolls v", "72850"},
            {"fallout 4", "377160"},
            {"minecraft", "1238430"},
            {"terraria", "105600"},
            {"among us", "945360"},
            {"valheim", "892970"},
            {"stardew valley", "413150"},
            {"pubg", "578080"},
            {"apex legends", "1172470"},
            {"destiny 2", "1085660"},
            {"warframe", "230410"},
            {"rocket league", "252950"},
            {"rust", "252490"},
            {"ark", "346110"},
            {"subnautica", "264710"},
            {"hollow knight", "367520"},
            {"dark souls 3", "374320"},
            {"elden ring", "1245620"},
            {"red dead redemption 2", "1174180"},
            {"hades", "1145360"},
            {"disco elysium", "632470"},
            {"ori and the blind forest", "261570"},
            {"celeste", "504230"},
            {"factorio", "427520"},
            {"rimworld", "294100"},
            {"cities skylines", "255710"},
            {"civilization vi", "289070"},
            {"total war", "364360"},
            {"football manager", "1569040"},
            {"fifa", "1313860"},
            {"call of duty", "1938090"},
            {"battlefield", "1517290"},
            {"assassin's creed", "1368820"},
            {"far cry", "552520"},
            {"watch dogs", "1208080"},
            {"borderlands 3", "397540"},
            {"monster hunter", "1334050"},
            {"resident evil", "1196590"},
            {"devil may cry", "601150"},
            {"final fantasy", "1313600"},
            {"tomb raider", "750920"},
            {"hitman", "1659040"},
            {"metro exodus", "412020"},
            {"control", "870780"},
            {"death stranding", "1190460"},
            {"no man's sky", "275850"},
            {"sea of thieves", "1172620"},
            {"forza horizon", "1551360"},
            {"microsoft flight simulator", "1250410"},
            {"age of empires", "1466860"}};

        // Convert search term to lowercase for matching
        std::string lowerSearchTerm = searchTerm;
        std::transform(lowerSearchTerm.begin(), lowerSearchTerm.end(), lowerSearchTerm.begin(), ::tolower);

        // Look for exact matches first
        auto it = popularGames.find(lowerSearchTerm);
        if (it != popularGames.end())
        {
            SteamGameInfo gameInfo = getGameInfo(it->second);
            if (!gameInfo.name.empty())
            {
                results.push_back(gameInfo);
                return results;
            }
        }

        // Look for partial matches
        std::set<std::string> addedAppIds; // Track added games to avoid duplicates
        for (const auto &game : popularGames)
        {
            if (game.first.find(lowerSearchTerm) != std::string::npos ||
                lowerSearchTerm.find(game.first) != std::string::npos)
            {

                // Skip if we already added this App ID
                if (addedAppIds.find(game.second) != addedAppIds.end())
                {
                    continue;
                }

                SteamGameInfo gameInfo = getGameInfo(game.second);
                if (!gameInfo.name.empty())
                {
                    results.push_back(gameInfo);
                    addedAppIds.insert(game.second);
                    // Limit to top 5 results to avoid overwhelming the user
                    if (results.size() >= 5)
                    {
                        break;
                    }
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error searching for games: " << e.what() << std::endl;
    }

    return results;
}

std::string SteamApiHelper::getGamePrice(const std::string &appId, const std::string &countryCode)
{
    try
    {
        std::string params = "appids=" + appId + "&cc=" + countryCode;
        std::string response = makeApiCall("/appdetails", params);

        return parseJsonValue(response, "price_overview");
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error fetching price for app ID " << appId << ": " << e.what() << std::endl;
        return "";
    }
}

SteamPlayerInfo SteamApiHelper::getPlayerInfo(const std::string &steamId)
{
    SteamPlayerInfo playerInfo;
    playerInfo.steamId = steamId;

    try
    {
        std::string params = "steamids=" + steamId + "&format=json";
        std::string response = makeApiCall("/ISteamUser/GetPlayerSummaries/v2/", params);

        playerInfo.personaName = parseJsonValue(response, "personaname");
        playerInfo.profileUrl = parseJsonValue(response, "profileurl");
        playerInfo.avatar = parseJsonValue(response, "avatarfull");
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error fetching player info: " << e.what() << std::endl;
    }

    return playerInfo;
}

std::vector<SteamGameInfo> SteamApiHelper::getOwnedGames(const std::string &steamId)
{
    std::vector<SteamGameInfo> ownedGames;

    try
    {
        std::string params = "steamid=" + steamId + "&include_appinfo=1&format=json";
        std::string response = makeApiCall("/IPlayerService/GetOwnedGames/v1/", params);

        // Parse owned games from response
        // This is a simplified implementation
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error fetching owned games: " << e.what() << std::endl;
    }

    return ownedGames;
}

std::string SteamApiHelper::resolveVanityUrl(const std::string &vanityUrl)
{
    try
    {
        std::string params = "vanityurl=" + vanityUrl + "&url_type=1&format=json";
        std::string response = makeApiCall("/ISteamUser/ResolveVanityURL/v1/", params);

        return parseJsonValue(response, "steamid");
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error resolving vanity URL: " << e.what() << std::endl;
        return "";
    }
}

bool SteamApiHelper::isValidSteamId(const std::string &steamId)
{
    // Basic validation - Steam IDs are 17-digit numbers
    if (steamId.length() != 17)
    {
        return false;
    }

    for (char c : steamId)
    {
        if (!std::isdigit(c))
        {
            return false;
        }
    }

    return true;
}

void SteamApiHelper::respectRateLimit()
{
    updateRateLimit();

    Config &config = Config::getInstance();
    int maxCallsPerMinute = std::stoi(config.get("API_RATE_LIMIT_PER_MINUTE"));

    if (callsInLastMinute >= maxCallsPerMinute)
    {
        std::cout << "Rate limit reached, waiting..." << std::endl;
        std::this_thread::sleep_for(std::chrono::minutes(1));
        updateRateLimit();
    }

    // Minimum delay between calls
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::system_clock::now().time_since_epoch())
                   .count();

    if (now - lastApiCall < 100)
    { // 100ms minimum between calls
        std::this_thread::sleep_for(std::chrono::milliseconds(100 - (now - lastApiCall)));
    }
}

std::string SteamApiHelper::makeApiCall(const std::string &endpoint, const std::string &params)
{
    respectRateLimit();

    Config &config = Config::getInstance();
    std::string baseUrl = config.get("STEAM_API_BASE_URL");
    if (baseUrl.empty())
    {
        baseUrl = "https://api.steampowered.com";
    }

    // For store API calls, use store API base URL
    if (endpoint.find("/appdetails") != std::string::npos)
    {
        baseUrl = config.get("STEAM_STORE_API_BASE_URL");
        if (baseUrl.empty())
        {
            baseUrl = "https://store.steampowered.com/api";
        }
        return NetworkUtils::fetchPageWithRetry(baseUrl + endpoint + "?" + params);
    }

    // For regular Steam Web API calls
    std::string additionalParams = params.empty() ? "format=json" : params + "&format=json";
    return NetworkUtils::fetchSteamApiData(endpoint, additionalParams);
}

void SteamApiHelper::updateRateLimit()
{
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::system_clock::now().time_since_epoch())
                   .count();

    // Remove calls older than 1 minute
    recentCalls.erase(
        std::remove_if(recentCalls.begin(), recentCalls.end(),
                       [now](long callTime)
                       { return (now - callTime) > 60000; }),
        recentCalls.end());

    callsInLastMinute = recentCalls.size();
    lastApiCall = now;
    recentCalls.push_back(now);
}

std::string SteamApiHelper::parseJsonValue(const std::string &json, const std::string &key)
{
    // Very basic JSON value extraction - in a real implementation, use a proper JSON library
    std::string searchKey = "\"" + key + "\"";
    size_t pos = json.find(searchKey);

    if (pos == std::string::npos)
    {
        return "";
    }

    pos += searchKey.length();
    pos = json.find(":", pos);
    if (pos == std::string::npos)
    {
        return "";
    }

    pos++;
    while (pos < json.length() && std::isspace(json[pos]))
    {
        pos++;
    }

    if (pos >= json.length())
    {
        return "";
    }

    if (json[pos] == '"')
    {
        pos++;
        size_t endPos = json.find('"', pos);
        if (endPos != std::string::npos)
        {
            return json.substr(pos, endPos - pos);
        }
    }
    else
    {
        size_t endPos = json.find_first_of(",}\n", pos);
        if (endPos != std::string::npos)
        {
            return json.substr(pos, endPos - pos);
        }
    }

    return "";
}

// Sales and pricing methods implementation
std::vector<SteamSaleInfo> SteamApiHelper::getCurrentSales(int limit)
{
    std::vector<SteamSaleInfo> sales;

    try
    {
        // Check some popular games for sales/pricing information
        std::vector<std::string> popularAppIds = {
            "292030",  // The Witcher 3
            "1091500", // Cyberpunk 2077
            "271590",  // GTA V
            "1245620", // Elden Ring
            "397540",  // Borderlands 3
            "632470",  // Disco Elysium
            "1174180", // Red Dead Redemption 2
            "1593500", // God of War
            "1449850", // Yu-Gi-Oh! Master Duel
            "346110"   // ARK: Survival Evolved
        };

        for (const auto &appId : popularAppIds)
        {
            if (sales.size() >= limit)
                break;

            try
            {
                SteamSaleInfo saleInfo = getSaleInfo(appId);
                if (!saleInfo.name.empty())
                {
                    // Add to sales list regardless of whether it's on sale or not
                    // This will show current pricing for popular games
                    sales.push_back(saleInfo);
                }
            }
            catch (const std::exception &e)
            {
                // Continue with next game if one fails
                continue;
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error fetching current sales: " << e.what() << std::endl;
    }

    return sales;
}

std::vector<SteamSaleInfo> SteamApiHelper::getFeaturedSales()
{
    return getCurrentSales(10);
}

std::vector<SteamSaleInfo> SteamApiHelper::getSpecialOffers()
{
    return getCurrentSales(15);
}

bool SteamApiHelper::isGameOnSale(const std::string &appId)
{
    try
    {
        std::string params = "appids=" + appId + "&cc=US&l=en";
        std::string response = makeApiCall("/appdetails", params);

        std::string priceOverview = parseJsonValue(response, "price_overview");
        std::string discountPercent = parseJsonValue(priceOverview, "discount_percent");

        return (!discountPercent.empty() && discountPercent != "0");
    }
    catch (const std::exception &e)
    {
        return false;
    }
}

SteamSaleInfo SteamApiHelper::getSaleInfo(const std::string &appId)
{
    SteamSaleInfo saleInfo;
    saleInfo.appId = appId;

    try
    {
        std::string params = "appids=" + appId + "&cc=US&l=en";
        std::string response = makeApiCall("/appdetails", params);

        // Parse the JSON response more carefully
        // Find the data section for this appId
        std::string dataKey = "\"" + appId + "\"";
        size_t dataPos = response.find(dataKey);
        if (dataPos == std::string::npos)
        {
            return saleInfo; // Return empty if not found
        }

        // Extract basic game info
        saleInfo.name = parseJsonValue(response, "name");
        saleInfo.headerImage = parseJsonValue(response, "header_image");
        saleInfo.currency = "USD";

        // Look for price_overview section
        std::string priceOverview = parseJsonValue(response, "price_overview");
        if (!priceOverview.empty())
        {
            saleInfo.currentPrice = parseJsonValue(priceOverview, "final_formatted");
            saleInfo.originalPrice = parseJsonValue(priceOverview, "initial_formatted");
            saleInfo.discountPercent = parseJsonValue(priceOverview, "discount_percent");

            // Check if it's actually on sale
            if (!saleInfo.discountPercent.empty() && saleInfo.discountPercent != "0")
            {
                // It's on sale, keep the discount info
                saleInfo.isHighlighted = (std::stoi(saleInfo.discountPercent) >= 25);
            }
            else
            {
                // Not on sale, use current price as the main price
                if (saleInfo.originalPrice.empty())
                {
                    saleInfo.originalPrice = saleInfo.currentPrice;
                }
                saleInfo.discountPercent = "";
                saleInfo.isHighlighted = false;
            }
        }
        else
        {
            // Check if it's free to play
            std::string isFree = parseJsonValue(response, "is_free");
            if (isFree == "true")
            {
                saleInfo.currentPrice = "Free to Play";
                saleInfo.originalPrice = "Free to Play";
            }
            saleInfo.isHighlighted = false;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error fetching sale info for app ID " << appId << ": " << e.what() << std::endl;
    }

    return saleInfo;
}
