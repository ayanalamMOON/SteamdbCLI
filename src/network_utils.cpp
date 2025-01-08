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

}
