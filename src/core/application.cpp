#include "application.hpp"

#include <spdlog/spdlog.h>

#include <chrono>
#include <thread>

Application::Application() {
    curl   = std::make_unique<Curl>();
    secret = std::make_unique<Secret>(SECRET_PATH);
    token  = std::make_unique<Token>();
    market = std::make_unique<Market>();
}

Application::~Application() {}

void Application::run() {
    // curl_easy_setopt(curl->get_handle(), CURLOPT_VERBOSE, 1L);
    
    token->generate(curl->get_handle(), *secret.get(), HOST);

    while (token->get_status() == "PENDING") {
        spdlog::info("[Application] Token is PENDING. Please open your Webull Mobile App to approve the login");
        
        std::this_thread::sleep_for(std::chrono::seconds(5));
        
        token->verify(curl->get_handle(), *secret.get(), HOST);
    }

    if (token->get_status() != "NORMAL") {
        spdlog::error("[Application] Token failed to activate. Status returned: {}", token->get_status());
        return;
    }

    spdlog::info("[Application] Token is ACTIVE! Proceeding to fetch market data.");

    market->fetch_tick_data(
        curl->get_handle(), 
        *secret.get(),
        token->get_handle(),
        "AAPL",
        "US_STOCK",
        "30",
        "RTH",
        HOST);
}