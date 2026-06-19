#include "application.hpp"

#include <spdlog/spdlog.h>

#include <chrono>
#include <thread>

Application::Application() {
    curl   = std::make_unique<Curl>();
    secret = std::make_unique<Secret>(SECRET_PATH);
    token  = std::make_unique<Token>(TOKEN_PATH, curl->get_handle(), *secret.get(), HOST);
    market = std::make_unique<Market>();
}

Application::~Application() {}

void Application::run() {

    spdlog::info("Token: {}", token->get_handle());
    
    token->verify(curl->get_handle(), *secret.get(), HOST);

    spdlog::info("[Application] Token is ACTIVE! Proceeding to fetch market data.");

    // market->fetch_tick_data(
    //     curl->get_handle(), 
    //     *secret.get(),
    //     token->get_handle(),
    //     "AAPL",
    //     "US_STOCK",
    //     "30",
    //     "RTH",
    //     HOST);
}