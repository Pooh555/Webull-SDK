#pragma once

#include "core/curl_pool.hpp"
#include "core/thread_pool.hpp"
#include "core/credentials.hpp"
#include "core/token.hpp"
#include "utilities/http.hpp"

#include <future>
#include <string>
#include <string_view>
#include <vector>

namespace wdk::client {

class MarketClient {
public:
    MarketClient(
              wdk::core::CurlPool&    pool,
              wdk::core::ThreadPool&  thread_pool,
        const wdk::core::Credentials& credentials, 
              std::string_view        host  = "", 
              std::string_view        token = "");
    ~MarketClient() = default;

    MarketClient(const MarketClient&)            = delete;
    MarketClient& operator=(const MarketClient&) = delete;

    struct MarketRequest {
        std::string            symbol                 { "" };
        std::string            symbols                { "" };
        std::string            category               { "" };
        std::string            timespan               { "" };
        std::optional<size_t>  count                  { std::nullopt };
        std::optional<bool>    real_time_required     { std::nullopt };
        std::string            trading_sessions       { "" };
        std::optional<uint8_t> depth                  { std::nullopt };
        std::optional<bool>    extended_hour_required { std::nullopt };
        std::optional<bool>    overnight_required     { std::nullopt };
    };

    [[nodiscard]] wdk::utilities::Response              fetch_tick_data(const MarketRequest& request);
    [[nodiscard]] std::future<wdk::utilities::Response> fetch_tick_data_async(const MarketRequest& request);
    [[nodiscard]] wdk::utilities::Response              fetch_snapshot_data(const MarketRequest& request);
    [[nodiscard]] std::future<wdk::utilities::Response> fetch_snapshot_data_async(const MarketRequest& request);
    [[nodiscard]] wdk::utilities::Response              fetch_quotes_data(const MarketRequest& request);
    [[nodiscard]] std::future<wdk::utilities::Response> fetch_quotes_data_async(const MarketRequest& request);
    [[nodiscard]] wdk::utilities::Response              fetch_footprint_data(const MarketRequest& request);
    [[nodiscard]] std::future<wdk::utilities::Response> fetch_footprint_data_async(const MarketRequest& request);
    [[nodiscard]] wdk::utilities::Response              fetch_historical_bars_data(const MarketRequest& request);
    [[nodiscard]] std::future<wdk::utilities::Response> fetch_historical_bars_data_async(const MarketRequest& request);  
    [[nodiscard]] wdk::utilities::Response              fetch_historical_batch_bars_data(const MarketRequest& request);
    [[nodiscard]] std::future<wdk::utilities::Response> fetch_historical_batch_bars_data_async(const MarketRequest& request);
private:
    static constexpr std::string_view TICK_PATH                  = "/openapi/market-data/stock/tick";
    static constexpr std::string_view SNAPSHOT_PATH              = "/openapi/market-data/stock/snapshot";
    static constexpr std::string_view QUOTES_PATH                = "/openapi/market-data/stock/quotes";
    static constexpr std::string_view FOOTPRINT_PATH             = "/openapi/market-data/stock/footprint";
    static constexpr std::string_view HISTORICAL_BATCH_BARS_PATH = "/openapi/market-data/stock/batch-bars";
    static constexpr std::string_view HISTORICAL_BARS_PATH       = "/openapi/market-data/stock/bars";

          wdk::core::CurlPool&    pool_;  
          wdk::core::ThreadPool&  thread_pool_;
    const wdk::core::Credentials& credentials_;
          std::string             host_         { "" };
          std::string             token_        { "" };

    [[nodiscard]] std::future<wdk::utilities::Response> execute_request_async(
        std::string                path,
        wdk::utilities::HttpMethod method,
        std::string                body_str = "");
};

}