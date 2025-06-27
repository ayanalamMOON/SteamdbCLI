#pragma once
#include <string>
#include <vector>

struct SteamGameInfo
{
    std::string appId;
    std::string name;
    std::string developer;
    std::string publisher;
    std::string releaseDate;
    std::string description;
    std::string price;
    std::string currency;
    bool isFree;
    bool isOnSale;
    std::string originalPrice;
    std::string discountPercent;
    std::string saleEndDate;
    std::vector<std::string> categories;
    std::vector<std::string> genres;
    std::string headerImage;
    std::string metacriticScore;
    std::string userReviews;
};

struct SteamPlayerInfo
{
    std::string steamId;
    std::string personaName;
    std::string profileUrl;
    std::string avatar;
    int communityVisibilityState;
    int profileState;
    long lastLogoff;
};

struct SteamSaleInfo
{
    std::string appId;
    std::string name;
    std::string currentPrice;
    std::string originalPrice;
    std::string discountPercent;
    std::string currency;
    std::string saleEndDate;
    std::string headerImage;
    bool isHighlighted;
};

class SteamApiHelper
{
public:
    // Initialize with API key validation
    bool initialize();

    // Game information methods
    SteamGameInfo getGameInfo(const std::string &appId);
    std::vector<SteamGameInfo> searchGames(const std::string &searchTerm);
    std::string getGamePrice(const std::string &appId, const std::string &countryCode = "US");

    // Sales and pricing methods
    std::vector<SteamSaleInfo> getCurrentSales(int limit = 20);
    std::vector<SteamSaleInfo> getFeaturedSales();
    std::vector<SteamSaleInfo> getSpecialOffers();
    bool isGameOnSale(const std::string &appId);
    SteamSaleInfo getSaleInfo(const std::string &appId);

    // Player information methods
    SteamPlayerInfo getPlayerInfo(const std::string &steamId);
    std::vector<SteamGameInfo> getOwnedGames(const std::string &steamId);

    // Utility methods
    std::string resolveVanityUrl(const std::string &vanityUrl);
    bool isValidSteamId(const std::string &steamId);

    // Rate limiting
    void respectRateLimit();

private:
    bool apiKeyValid;
    long lastApiCall;
    int callsInLastMinute;
    std::vector<long> recentCalls;

    // Helper methods
    std::string makeApiCall(const std::string &endpoint, const std::string &params = "");
    void updateRateLimit();
    std::string parseJsonValue(const std::string &json, const std::string &key);
};
