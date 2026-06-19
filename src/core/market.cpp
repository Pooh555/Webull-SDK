#include "market.hpp"

#include "utilities/utilities.hpp"

#include <spdlog/spdlog.h>

void Market::fetch_tick_data(
          CURL*             curl, 
    const Secret&           secret, 
    const std::string&      token, 
    const std::string&      symbol,
    const std::string&      category,
    const std::string&      count,
    const std::string&      session,
    const std::string_view& host)  {
    std::string timestamp = utilities::get_utc_timestamp();
    std::string nonce     = utilities::generate_nonce(26uz);
    
    std::vector<std::pair<std::string, std::string>> query_parameters {
        {"category", category},
        {"count", count},
        {"symbol", symbol},
        {"trading_sessions", session}
    };

    std::string signature = utilities::generate_openapi_signature(
        curl, secret.get_key(), secret.get_secret(), nonce, timestamp, host, TICK_PATH, query_parameters, ""
    );

    std::string url = "https://" + static_cast<std::string>(host) + static_cast<std::string>(TICK_PATH) + 
                      "?symbol=" + symbol + 
                      "&category=" + category + 
                      "&count=" + count + 
                      "&trading_sessions=" + session;
                      
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");

    curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, ("x-app-key: " + secret.get_key()).c_str());
    headers = curl_slist_append(headers, ("x-timestamp: " + timestamp).c_str());
    headers = curl_slist_append(headers, "x-signature-version: 1.0");
    headers = curl_slist_append(headers, "x-signature-algorithm: HMAC-SHA1");
    headers = curl_slist_append(headers, ("x-signature-nonce: " + nonce).c_str());
    headers = curl_slist_append(headers, ("x-access-token: " + token).c_str());
    headers = curl_slist_append(headers, "x-version: v2");
    headers = curl_slist_append(headers, ("x-signature: " + signature).c_str());

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    
    std::string response_message = "";

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, utilities::write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_message);

    CURLcode response_code = curl_easy_perform(curl);

    if (response_code == CURLE_OK) {
        long int http_code { 0L };

        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        
        if (http_code == 200L) {
            spdlog::info("[Market] Successfully fetch tick market data");
            
            try {
                auto json_response = nlohmann::json::parse(response_message);
                spdlog::info("[Market] Tick market data:\n{}", json_response.dump(4));
            } catch (const nlohmann::json::parse_error& e) {
                spdlog::warn("[Market] Failed to parse JSON verification response: {}", e.what());
                spdlog::info("[Market] Raw Response: {}", response_message);
            }
        } else {
            spdlog::error("[Market] API rejected verification request. HTTP {}: {}", http_code, response_message);
        }
    } else {
        spdlog::error("[Market] Curl verification request failed: {}", curl_easy_strerror(response_code));
    }
}