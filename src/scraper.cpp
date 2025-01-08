#include "scraper.h"
#include "network_utils.h"
#include "rate_limiter.h"
#include <curl/curl.h>
#include <regex>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

Scraper::Scraper() : rateLimiter(std::make_unique<RateLimiter>()) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

Scraper::~Scraper() {
    curl_global_cleanup();
}

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
            throw NetworkError("Failed to fetch page");
        }
    }
    
    return readBuffer;
}

GameData Scraper::searchGame(const std::string& gameName) {
    std::string baseUrl = "https://steamdb.info/search/?q=";
    std::string url = NetworkUtils::constructUrl(baseUrl, gameName);
    std::string page = fetchPage(url);
    return parseGameData(page);
}

GameData Scraper::parseGameData(const std::string& html) {
    GameData gameData;

    std::regex nameRegex("<td class=\"b\">(.*?)</td>");
    std::regex appIdRegex("<td class=\"b\">(\\d+)</td>");
    std::regex currentPriceRegex("<td class=\"price\">(.*?)</td>");
    std::regex lowestPriceRegex("<td class=\"price\">(.*?)</td>");
    std::regex metacriticRegex("<td class=\"score\">(.*?)</td>");
    std::regex releaseDateRegex("<td class=\"date\">(.*?)</td>");
    std::regex tagsRegex("<a class=\"tag\" href=\".*?\">(.*?)</a>");
    std::regex descriptionRegex("<div class=\"description\">(.*?)</div>");

    std::smatch match;

    if (std::regex_search(html, match, nameRegex)) {
        gameData.name = match[1].str();
    }
    if (std::regex_search(html, match, appIdRegex)) {
        gameData.appId = match[1].str();
    }
    if (std::regex_search(html, match, currentPriceRegex)) {
        gameData.currentPrice = match[1].str();
    }
    if (std::regex_search(html, match, lowestPriceRegex)) {
        gameData.lowestPrice = match[1].str();
    }
    if (std::regex_search(html, match, metacriticRegex)) {
        gameData.metacritic = match[1].str();
    }
    if (std::regex_search(html, match, releaseDateRegex)) {
        gameData.releaseDate = match[1].str();
    }
    if (std::regex_search(html, match, descriptionRegex)) {
        gameData.description = match[1].str();
    }

    auto tagsBegin = std::sregex_iterator(html.begin(), html.end(), tagsRegex);
    auto tagsEnd = std::sregex_iterator();

    for (std::sregex_iterator i = tagsBegin; i != tagsEnd; ++i) {
        gameData.tags.push_back((*i)[1].str());
    }

    return gameData;
}
