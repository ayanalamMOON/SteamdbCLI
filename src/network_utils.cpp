#include "network_utils.h"
#include <curl/curl.h>
#include <sstream>
#include <iomanip>
#include "error_handling.h"
#include <thread>
#include <chrono>

// Callback function to write data received from the server to a string
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

namespace NetworkUtils {

    // Fetch the HTML content of a web page
    std::string fetchPage(const std::string& url) {
        CURL* curl;
        CURLcode res;
        std::string readBuffer;
        curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            if(res != CURLE_OK) {
                throw NetworkError("Failed to fetch page: " + std::string(curl_easy_strerror(res)));
            }
        } else {
            throw NetworkError("Failed to initialize CURL for fetching page");
        }
        return readBuffer;
    }

    // Check if there is an active internet connection
    bool checkInternetConnection() {
        CURL* curl;
        CURLcode res;
        curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, "http://www.google.com");
            curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            if(res == CURLE_OK) {
                return true;
            } else {
                throw NetworkError("Failed to check internet connection: " + std::string(curl_easy_strerror(res)));
            }
        } else {
            throw NetworkError("Failed to initialize CURL for internet connection check");
        }
    }

    // Encode a URL string to make it safe for use in a URL
    std::string urlEncode(const std::string& str) {
        std::ostringstream encoded;
        for (char c : str) {
            if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
                encoded << c;
            } else {
                encoded << '%' << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << (int)c;
            }
        }
        return encoded.str();
    }

    // Construct a URL by combining a base URL and a query string
    std::string constructUrl(const std::string& baseUrl, const std::string& query) {
        try {
            return baseUrl + urlEncode(query);
        } catch (const std::exception& e) {
            throw NetworkError("Failed to construct URL: " + std::string(e.what()));
        }
    }

    // Fetch the HTML content of a web page with retry mechanism
    std::string fetchPageWithRetry(const std::string& url, int maxRetries, int retryDelay) {
        int attempt = 0;
        while (attempt < maxRetries) {
            try {
                return fetchPage(url);
            } catch (const NetworkError& e) {
                if (attempt < maxRetries - 1) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(retryDelay));
                } else {
                    throw;
                }
            }
            ++attempt;
        }
        throw NetworkError("Failed to fetch page after " + std::to_string(maxRetries) + " attempts");
    }

}

// Declare the global error handler function as an external function
extern void globalErrorHandler();
