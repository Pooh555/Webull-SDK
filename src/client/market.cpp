#include <client/market.hpp>

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include <format>
#include <memory>

namespace wdk::client {

MarketClient::MarketClient(
          wdk::core::CurlPool&    pool,
          wdk::core::ThreadPool&  thread_pool,
    const wdk::core::Credentials& credentials,
          std::string_view       host,
          std::string_view       token)
    : pool_(pool),
      thread_pool_(thread_pool),
      credentials_(credentials),
      host_(host),
      token_(token) {}

wdk::utilities::Response MarketClient::fetch_tick_data(const MarketRequest& request) {
    std::string path { TICK_PATH };

    bool first_parameter { true };

    auto append_parameter = [&](std::string_view key, std::string_view value) {
        if (value.empty()) return;
        path           += first_parameter ? '?' : '&';
        path           += std::format("{}={}", key, value);
        first_parameter = false;
    };

    append_parameter("symbol", request.symbol);
    append_parameter("category", request.category);
    
    if (request.count.has_value()) {
        append_parameter("count", std::to_string(request.count.value()));
    }

    append_parameter("trading_sessions", request.trading_sessions);

    return wdk::utilities::execute_request(
        pool_,
        credentials_,
        host_,
        path,
        wdk::utilities::HttpMethod::GET,
        "",
        token_
    );
}

std::future<wdk::utilities::Response> MarketClient::fetch_tick_data_async(const MarketRequest& request) {
    std::string path { TICK_PATH };

    bool first_parameter { true };

    auto append_parameter = [&](std::string_view key, std::string_view value) {
        if (value.empty()) return;

        path           += first_parameter ? '?' : '&';
        path           += std::format("{}={}", key, value);
        first_parameter = false;
    };

    append_parameter("symbol", request.symbol);
    append_parameter("category", request.category);
    
    if (request.count.has_value()) {
        append_parameter("count", std::to_string(request.count.value()));
    }
    
    append_parameter("trading_sessions", request.trading_sessions);
    
    return execute_request_async(path, wdk::utilities::HttpMethod::GET);
}

wdk::utilities::Response MarketClient::fetch_snapshot_data(const MarketRequest& request) {
    std::string path { SNAPSHOT_PATH };

    bool first_parameter { true };

    auto append_parameter = [&](std::string_view key, std::string_view value) {
        if (value.empty()) return;

        path           += first_parameter ? '?' : '&';
        path           += std::format("{}={}", key, value);
        first_parameter = false;
    };

    append_parameter("symbols", request.symbols);
    append_parameter("category", request.category);

    if (request.extended_hour_required.has_value()) {
        append_parameter("extend_hour_required", request.extended_hour_required.value() ? "true" : "false");
    }

    if (request.overnight_required.has_value()) {
        append_parameter("overnight_required", request.overnight_required.value() ? "true" : "false");
    }

    return wdk::utilities::execute_request(
        pool_,
        credentials_,
        host_,
        path,
        wdk::utilities::HttpMethod::GET,
        "",
        token_
    );
}

std::future<wdk::utilities::Response> MarketClient::fetch_snapshot_data_async(const MarketRequest& request) {
    std::string path { SNAPSHOT_PATH };

    bool first_parameter { true };

    auto append_parameter = [&](std::string_view key, std::string_view value) {
        if (value.empty()) return;

        path           += first_parameter ? '?' : '&';
        path           += std::format("{}={}", key, value);
        first_parameter = false;
    };

    append_parameter("symbols", request.symbols);
    append_parameter("category", request.category);

    if (request.extended_hour_required.has_value()) {
        append_parameter("extend_hour_required", request.extended_hour_required.value() ? "true" : "false");
    }

    if (request.overnight_required.has_value()) {
        append_parameter("overnight_required", request.overnight_required.value() ? "true" : "false");
    }
    
    return execute_request_async(path, wdk::utilities::HttpMethod::GET);
}

wdk::utilities::Response MarketClient::fetch_quotes_data(const MarketRequest& request) {
    std::string path { QUOTES_PATH };

    bool first_parameter { true };

    auto append_parameter = [&](std::string_view key, std::string_view value) {
        if (value.empty()) return;

        path           += first_parameter ? '?' : '&';
        path           += std::format("{}={}", key, value);
        first_parameter = false;
    };

    append_parameter("symbol", request.symbol);
    append_parameter("category", request.category);
    
    if (request.depth.has_value()) {
        append_parameter("depth", std::to_string(request.depth.value()));
    }

    if (request.overnight_required.has_value()) {
        append_parameter("overnight_required", request.overnight_required.value() ? "true" : "false");
    }

    return wdk::utilities::execute_request(
        pool_,
        credentials_,
        host_,
        path,
        wdk::utilities::HttpMethod::GET,
        "",
        token_
    );
}

std::future<wdk::utilities::Response> MarketClient::fetch_quotes_data_async(const MarketRequest& request) {
    std::string path { QUOTES_PATH };

    bool first_parameter { true };

    auto append_parameter = [&](std::string_view key, std::string_view value) {
        if (value.empty()) return;

        path           += first_parameter ? '?' : '&';
        path           += std::format("{}={}", key, value);
        first_parameter = false;
    };

    append_parameter("symbol", request.symbol);
    append_parameter("category", request.category);
    
    if (request.depth.has_value()) {
        append_parameter("depth", std::to_string(request.depth.value()));
    }

    if (request.overnight_required.has_value()) {
        append_parameter("overnight_required", request.overnight_required.value() ? "true" : "false");
    }

    return execute_request_async(path, wdk::utilities::HttpMethod::GET);
}

wdk::utilities::Response MarketClient::fetch_footprint_data(const MarketRequest& request) {
    std::string path { FOOTPRINT_PATH };

    bool first_parameter { true };

    auto append_parameter = [&](std::string_view key, std::string_view value) {
        if (value.empty()) return;

        path           += first_parameter ? '?' : '&';
        path           += std::format("{}={}", key, value);
        first_parameter = false;
    };

    append_parameter("symbols", request.symbols);
    append_parameter("category", request.category);
    append_parameter("timespan", request.timespan);

    if (request.count.has_value()) {
        append_parameter("count", std::to_string(request.count.value()));
    }
    
    if (request.real_time_required.has_value()) {
        append_parameter("real_time_required", request.real_time_required.value() ? "true" : "false");
    }

    append_parameter("trading_sessions", request.trading_sessions);

    return wdk::utilities::execute_request(
        pool_,
        credentials_,
        host_,
        path,
        wdk::utilities::HttpMethod::GET,
        "",
        token_
    );
}

std::future<wdk::utilities::Response> MarketClient::fetch_footprint_data_async(const MarketRequest& request) {
    std::string path { FOOTPRINT_PATH };

    bool first_parameter { true };

    auto append_parameter = [&](std::string_view key, std::string_view value) {
        if (value.empty()) return;

        path           += first_parameter ? '?' : '&';
        path           += std::format("{}={}", key, value);
        first_parameter = false;
    };

    append_parameter("symbols", request.symbols);
    append_parameter("category", request.category);
    append_parameter("timespan", request.timespan);

    if (request.count.has_value()) {
        append_parameter("count", std::to_string(request.count.value()));
    }
    
    if (request.real_time_required.has_value()) {
        append_parameter("real_time_required", request.real_time_required.value() ? "true" : "false");
    }

    append_parameter("trading_sessions", request.trading_sessions);

    return execute_request_async(path, wdk::utilities::HttpMethod::GET);
}

wdk::utilities::Response MarketClient::fetch_historical_bars_data(const MarketRequest& request) {
    std::string path { HISTORICAL_BARS_PATH };

    bool first_parameter { true };

    auto append_parameter = [&](std::string_view key, std::string_view value) {
        if (value.empty()) return;

        path           += first_parameter ? '?' : '&';
        path           += std::format("{}={}", key, value);
        first_parameter = false;
    };

    append_parameter("symbol", request.symbol);
    append_parameter("category", request.category);
    append_parameter("timespan", request.timespan);

    if (request.count.has_value()) {
        append_parameter("count", std::to_string(request.count.value()));
    }
    
    if (request.real_time_required.has_value()) {
        append_parameter("real_time_required", request.real_time_required.value() ? "true" : "false");
    }

    append_parameter("trading_sessions", request.trading_sessions);

    return wdk::utilities::execute_request(
        pool_,
        credentials_,
        host_,
        path,
        wdk::utilities::HttpMethod::GET,
        "",
        token_
    );
}

std::future<wdk::utilities::Response> MarketClient::fetch_historical_bars_data_async(const MarketRequest& request) {
    std::string path { HISTORICAL_BARS_PATH };

    bool first_parameter { true };

    auto append_parameter = [&](std::string_view key, std::string_view value) {
        if (value.empty()) return;

        path           += first_parameter ? '?' : '&';
        path           += std::format("{}={}", key, value);
        first_parameter = false;
    };

    append_parameter("symbol", request.symbol);
    append_parameter("category", request.category);
    append_parameter("timespan", request.timespan);

    if (request.count.has_value()) {
        append_parameter("count", std::to_string(request.count.value()));
    }
    
    if (request.real_time_required.has_value()) {
        append_parameter("real_time_required", request.real_time_required.value() ? "true" : "false");
    }

    append_parameter("trading_sessions", request.trading_sessions);

    return execute_request_async(path, wdk::utilities::HttpMethod::GET);
}

wdk::utilities::Response MarketClient::fetch_historical_batch_bars_data(const MarketRequest& request) {
    nlohmann::json           request_body;
    std::vector<std::string> symbol_list;
    std::stringstream        ss(request.symbols);
    std::string              item;
    
    while (std::getline(ss, item, ',')) {
        if (!item.empty()) {
            symbol_list.push_back(item);
        }
    }

    request_body["symbols"]  = symbol_list;
    request_body["category"] = request.category;
    request_body["timespan"] = request.timespan;

    if (request.count.has_value()) {
        request_body["count"] = request.count.value();
    }

    if (request.real_time_required.has_value()) {
        request_body["real_time_required"] = request.real_time_required.value();
    }

    if (!request.trading_sessions.empty()) {
        request_body["trading_sessions"] = request.trading_sessions;
    }

    return wdk::utilities::execute_request(
        pool_, 
        credentials_, 
        host_, 
        HISTORICAL_BATCH_BARS_PATH, 
        wdk::utilities::HttpMethod::POST, 
        request_body.dump(), 
        token_
    );
}

std::future<wdk::utilities::Response> MarketClient::fetch_historical_batch_bars_data_async(const MarketRequest& request) {
    nlohmann::json           request_body;
    std::vector<std::string> symbol_list;
    std::stringstream        ss(request.symbols);
    std::string              item;

    while (std::getline(ss, item, ',')) {
        if (!item.empty()) {
            symbol_list.push_back(item);
        }
    }

    request_body["symbols"]  = symbol_list;
    request_body["category"] = request.category;
    request_body["timespan"] = request.timespan;

    if (request.count.has_value()) {
        request_body["count"] = request.count.value();
    }

    if (request.real_time_required.has_value()) {
        request_body["real_time_required"] = request.real_time_required.value();
    }

    if (!request.trading_sessions.empty()) {
        request_body["trading_sessions"] = request.trading_sessions;
    }

    return execute_request_async(
        std::string{ HISTORICAL_BATCH_BARS_PATH }, 
        wdk::utilities::HttpMethod::POST, 
        request_body.dump()
    );
}

std::future<wdk::utilities::Response> MarketClient::execute_request_async(
    std::string                path,
    wdk::utilities::HttpMethod method,
    std::string                body_str) {
    return thread_pool_.enqueue(
        [this, path = std::move(path), method, body_str = std::move(body_str)]() {
            return wdk::utilities::execute_request(
                pool_,
                credentials_,
                host_,
                path,
                method,
                body_str,
                token_
            );
        }
    );
}

}