#pragma once
#include <string>
#include "error_handling.h"

namespace NetworkUtils
{
    // Check if there is an active internet connection
    bool checkInternetConnection();

    // Encode a URL string to make it safe for use in a URL
    std::string urlEncode(const std::string &str);

    // Construct a URL by combining a base URL and a query string
    std::string constructUrl(const std::string &baseUrl, const std::string &query);

    // Retry failed network requests with exponential backoff
    std::string fetchPageWithRetry(const std::string &url, int maxRetries = 3, int retryDelay = 2000);

    // Steam API specific functions
    std::string buildSteamApiUrl(const std::string &endpoint, const std::string &additionalParams = "");
    std::string fetchSteamApiData(const std::string &endpoint, const std::string &additionalParams = "");
    bool validateSteamApiKey(const std::string &apiKey);
}
