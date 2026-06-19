#pragma once

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <filesystem>
#include <string>

namespace utilities {
    
void read_json(nlohmann::json* json, const std::filesystem::path& input_path);
void write_json(const nlohmann::json& json, const std::filesystem::path& output_path);

[[nodiscard]] std::string get_utc_timestamp();
[[nodiscard]] std::string generate_nonce(size_t length = 0uz);
[[nodiscard]] std::string generate_openapi_signature(
          CURL*                                             curl,
    const std::string&                                      app_key,
    const std::string&                                      app_secret,
    const std::string&                                      nonce,
    const std::string&                                      timestamp,
    const std::string_view&                                 host,
    const std::string_view&                                 request_path,
    const std::vector<std::pair<std::string, std::string>>& query_params,
    const std::string&                                      request_body);
[[nodiscard]] std::string compute_hmac_sha1(const std::string& key, const std::string& message);
[[nodiscard]] std::string compute_hmac_sha256(const std::string& key, const std::string& message);
[[nodiscard]] std::string compute_md5(const std::string& data);

size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);

}