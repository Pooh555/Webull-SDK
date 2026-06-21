#pragma once

#include <filesystem>
#include <string>

namespace wdk::core {

class Credentials {
public:
    Credentials(const std::filesystem::path& credentials_path);
    ~Credentials() = default;

    Credentials(const Credentials&)            = default;
    Credentials& operator=(const Credentials&) = default;
    
    [[nodiscard]] const std::string& get_id()     const { return id_; }
    [[nodiscard]] const std::string& get_key()    const { return key_; }
    [[nodiscard]] const std::string& get_secret() const { return secret_; }
private:
    std::string id_     { "" };
    std::string key_    { "" };
    std::string secret_ { "" };
};

}