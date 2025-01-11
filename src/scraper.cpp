#include "scraper.h"
#include "network_utils.h"
#include <curl/curl.h>
#include <regex>
#include "error_handling.h"
#include "logger.h"

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
    std::string baseUrl = "https://steamdb.info/search/";
    std::string url = baseUrl + "?term=" + NetworkUtils::urlEncode(gameName);
    std::string page;
    try {
        page = fetchPage(url);
    } catch (const NetworkError& e) {
        throw NetworkError("Network error while searching for game: " + gameName + ". " + e.what());
    } catch (const std::exception& e) {
        throw std::runtime_error("Unexpected error while searching for game: " + gameName + ". " + e.what());
    }
    return parseGameData(page);
}

// Parse the HTML content to extract game data
GameData Scraper::parseGameData(const std::string& html) {
    GameData gameData;

    // Check if search returned any results
    if (html.find("No results found") != std::string::npos) {
        throw std::runtime_error("No games found matching the search criteria");
    }

    // Extract from search results table
    std::regex tableRowRegex("<tr[^>]*data-appid=\"(\\d+)\"[^>]*>\\s*"
                            "<td[^>]*>.*?</td>\\s*"  // Type column
                            "<td[^>]*><a[^>]*>(.*?)</a>.*?</td>\\s*"  // Name column
                            "<td[^>]*>(.*?)</td>");  // Release date column

    std::smatch tableMatch;
    if (std::regex_search(html, tableMatch, tableRowRegex)) {
        gameData.appId = tableMatch[1].str();
        gameData.name = tableMatch[2].str();
        gameData.releaseDate = tableMatch[3].str();

        // Fetch detailed page for the game
        std::string detailUrl = "https://steamdb.info/app/" + gameData.appId + "/";
        try {
            std::string detailPage = fetchPage(detailUrl);
            
            // Extract price
            std::regex priceRegex("<td[^>]*>Current Price:</td>\\s*<td[^>]*>(.*?)</td>");
            std::smatch priceMatch;
            if (std::regex_search(detailPage, priceMatch, priceRegex)) {
                gameData.currentPrice = priceMatch[1].str();
            }

            // Extract lowest price
            std::regex lowestPriceRegex("<td[^>]*>Lowest Price:</td>\\s*<td[^>]*>(.*?)</td>");
            std::smatch lowestMatch;
            if (std::regex_search(detailPage, lowestMatch, lowestPriceRegex)) {
                gameData.lowestPrice = lowestMatch[1].str();
            }

            // Extract Metacritic score
            std::regex metacriticRegex("Metacritic Score:</td>\\s*<td[^>]*><a[^>]*>(\\d+)</a>");
            std::smatch metacriticMatch;
            if (std::regex_search(detailPage, metacriticMatch, metacriticRegex)) {
                gameData.metacritic = metacriticMatch[1].str();
            }

            // Extract tags
            std::regex tagsRegex("<a[^>]*class=\"app-tag\"[^>]*>(.*?)</a>");
            std::smatch tagsMatch;
            std::string::const_iterator searchStart(detailPage.cbegin());
            while (std::regex_search(searchStart, detailPage.cend(), tagsMatch, tagsRegex)) {
                gameData.tags.push_back(tagsMatch[1].str());
                searchStart = tagsMatch.suffix().first;
            }

            // Extract review score
            std::regex reviewRegex("User Reviews:</td>\\s*<td[^>]*>(.*?)</td>");
            std::smatch reviewMatch;
            if (std::regex_search(detailPage, reviewMatch, reviewRegex)) {
                gameData.reviewScore = reviewMatch[1].str();
            }
        } catch (const std::exception& e) {
            // If detail page fails, we still return basic info from search
            std::cerr << "Warning: Could not fetch detailed information: " << e.what() << std::endl;
        }
    } else {
        // Detailed error handling for parsing failures
        std::regex errorRegex("<div[^>]*class=\"error\"[^>]*>(.*?)</div>");
        std::smatch errorMatch;
        if (std::regex_search(html, errorMatch, errorRegex)) {
            throw ParsingError("Parsing error: " + errorMatch[1].str());
        } else {
            throw ParsingError("Could not parse search results. HTML content may have changed.");
        }
    }

    return gameData;
}

// Global error handler function to catch and log unhandled exceptions
void globalErrorHandler() {
    try {
        throw; // Re-throw the current exception
    } catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        // Log the error (assuming a logger instance is available)
        // logger.error("Unhandled exception: " + std::string(e.what()));
    } catch (...) {
        std::cerr << "Unhandled unknown exception" << std::endl;
        // Log the error (assuming a logger instance is available)
        // logger.error("Unhandled unknown exception");
    }
}
