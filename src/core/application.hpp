#pragma once

#include "curl.hpp"
#include "market.hpp"
#include "token.hpp"
#include "secret/secret.hpp"

#include <memory>

class Application {
public:
    Application();
    ~Application();

    Application(const Application&)            = delete;
    Application& operator=(const Application&) = delete;

    void run();
private:
    static constexpr std::string_view HOST        = "api.webull.co.th";
    static constexpr std::string_view SECRET_PATH = "/home/Pooh555/programming/Webull-Trading-Bot/secret.json";

    std::unique_ptr<Curl>   curl;
    std::unique_ptr<Secret> secret;
    std::unique_ptr<Token>  token;
    std::unique_ptr<Market> market;
};