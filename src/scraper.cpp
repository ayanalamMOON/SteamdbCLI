#include "scraper.h"
#include "network_utils.h"
#include <curl/curl.h>
#include <regex>

// Callback function to write data received from the server to a string
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Constructor to initialize the scraper and rate limiter
Scraper::Scraper() : rateLimiter(std::make_unique<RateLimiter>()) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

// Destructor to clean up resources
Scraper::~Scraper() {
    curl_global_cleanup();
}

// Fetch the HTML content of a web page
std::string Scraper::fetchPage(const std::string& url) {
    rateLimiter->waitForNext();
    
    CURL* curl = curl_easy_init();
    std::string readBuffer;
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "SteamDB CLI/1.0");
        
        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        
        if (res != CURLE_OK) {
            throw NetworkError("Failed to fetch page: " + std::string(curl_easy_strerror(res)));
        }
    } else {
        throw NetworkError("Failed to initialize CURL");
    }
    
    return readBuffer;
}

// Search for a game by name and return its data
GameData Scraper::searchGame(const std::string& gameName) {
    std::string baseUrl = "https://steamdb.info/search/?q=";
    std::string url = NetworkUtils::constructUrl(baseUrl, gameName);
    std::string page = fetchPage(url);
    return parseGameData(page);
}

// Parse the HTML content to extract game data
GameData Scraper::parseGameData(const std::string& html) {
    GameData gameData;

    // Extract game name
    std::regex nameRegex("<h1 class=\"page-title\">(.*?)</h1>");
    std::smatch nameMatch;
    if (std::regex_search(html, nameMatch, nameRegex)) {
        gameData.name = nameMatch[1].str();
    } else {
        throw std::runtime_error("Failed to parse game name");
    }

    // Extract app ID
    std::regex appIdRegex("data-appid=\"(\\d+)\"");
    std::smatch appIdMatch;
    if (std::regex_search(html, appIdMatch, appIdRegex)) {
        gameData.appId = appIdMatch[1].str();
    } else {
        throw std::runtime_error("Failed to parse app ID");
    }

    // Extract current price
    std::regex currentPriceRegex("<td class=\"current-price\">(.*?)</td>");
    std::smatch currentPriceMatch;
    if (std::regex_search(html, currentPriceMatch, currentPriceRegex)) {
        gameData.currentPrice = currentPriceMatch[1].str();
    } else {
        throw std::runtime_error("Failed to parse current price");
    }

    // Extract lowest price
    std::regex lowestPriceRegex("<td class=\"lowest-price\">(.*?)</td>");
    std::smatch lowestPriceMatch;
    if (std::regex_search(html, lowestPriceMatch, lowestPriceRegex)) {
        gameData.lowestPrice = lowestPriceMatch[1].str();
    } else {
        throw std::runtime_error("Failed to parse lowest price");
    }

    // Extract Metacritic score
    std::regex metacriticRegex("<div class=\"score\">(.*?)</div>");
    std::smatch metacriticMatch;
    if (std::regex_search(html, metacriticMatch, metacriticRegex)) {
        gameData.metacritic = metacriticMatch[1].str();
    } else {
        throw std::runtime_error("Failed to parse Metacritic score");
    }

    // Extract release date
    std::regex releaseDateRegex("<td class=\"date\">(.*?)</td>");
    std::smatch releaseDateMatch;
    if (std::regex_search(html, releaseDateMatch, releaseDateRegex)) {
        gameData.releaseDate = releaseDateMatch[1].str();
    } else {
        throw std::runtime_error("Failed to parse release date");
    }

    // Extract tags
    std::regex tagsRegex("<a class=\"tag\" href=\".*?\">(.*?)</a>");
    std::smatch tagsMatch;
    std::string::const_iterator searchStart(html.cbegin());
    while (std::regex_search(searchStart, html.cend(), tagsMatch, tagsRegex)) {
        gameData.tags.push_back(tagsMatch[1].str());
        searchStart = tagsMatch.suffix().first;
    }

    // Extract description
    std::regex descriptionRegex("<div class=\"game-description\">(.*?)</div>");
    std::smatch descriptionMatch;
    if (std::regex_search(html, descriptionMatch, descriptionRegex)) {
        gameData.description = descriptionMatch[1].str();
    } else {
        throw std::runtime_error("Failed to parse description");
    }

    return gameData;
}
