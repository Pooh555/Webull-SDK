#pragma once

#include "core/credentials.hpp"
#include "core/curl_pool.hpp" 

#include <string>
#include <string_view>

namespace wdk::core {

class Token {
public:
    Token(
        const std::filesystem::path& token_path,
              CurlPool&              pool,   
        const Credentials&           credentials,
        const std::string_view&      host);
    ~Token() = default;
    
    Token(const Token&)            = delete;
    Token& operator=(const Token&) = delete;

    Token(Token&&)            noexcept = default;
    Token& operator=(Token&&) noexcept = default;

    void generate(
              CurlPool&         pool,        
        const Credentials&      credentials, 
        const std::string_view& host);

    void verify(
              CurlPool&         pool,        
        const Credentials&      credentials, 
        const std::string_view& host);

    [[nodiscard]] std::string get_handle() const { return token_; }
    [[nodiscard]] std::string get_status() const { return status_; }
    [[nodiscard]] bool        is_valid()   const { return !token_.empty(); }
private:
    static constexpr std::string_view CREATE_PATH { "/openapi/auth/token/create" };
    static constexpr std::string_view VERIFY_PATH { "/openapi/auth/token/check" };

    std::string token_  { "" };
    std::string status_ { "" };
};

}