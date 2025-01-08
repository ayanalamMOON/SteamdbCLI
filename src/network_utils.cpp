#include "network_utils.h"
#include <curl/curl.h>
#include <sstream>
#include <iomanip>

namespace NetworkUtils {

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
            }
        }
        return false;
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
        return baseUrl + urlEncode(query);
    }

    // Fetch the HTML content of a web page and handle HTTP status codes
    std::string fetchPage(const std::string& url) {
        CURL* curl = curl_easy_init();
        std::string readBuffer;

        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "SteamDB CLI/1.0");

            CURLcode res = curl_easy_perform(curl);
            long http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            curl_easy_cleanup(curl);

            if (res != CURLE_OK) {
                throw NetworkError("Failed to fetch page: " + std::string(curl_easy_strerror(res)));
            }

            if (http_code != 200) {
                throw NetworkError("HTTP error: " + std::to_string(http_code));
            }
        } else {
            throw NetworkError("Failed to initialize CURL");
        }

        return readBuffer;
    }

}
