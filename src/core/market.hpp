#pragma once

#include "token.hpp"
#include "secret/secret.hpp"

#include <curl/curl.h>

#include <string>
#include <string_view>

class Market {
public:
    Market()  = default;
    ~Market() = default;

    Market(const Market&)            = delete;
    Market& operator=(const Market&) = delete;

    void fetch_tick_data(
              CURL*             curl, 
        const Secret&           secret, 
        const std::string&      token, 
        const std::string&      symbol,
        const std::string&      category,
        const std::string&      count,
        const std::string&      session,
        const std::string_view& host);
private:
    static constexpr std::string_view TICK_PATH = "/openapi/market-data/stock/tick";
};