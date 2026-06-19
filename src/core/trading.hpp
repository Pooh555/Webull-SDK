#pragma once

#include "secret/secret.hpp"

#include <curl/curl.h>

#include <string>
#include <optional>

namespace trading {

struct OrderRequest {
    std::string           account_id              = "";            
    std::string           combo_type              = "";  
    std::string           client_order_id         = "";  
    std::string           instrument_type         = "";  
    std::string           market                  = "";  
    std::string           symbol                  = "";  
    std::string           order_type              = "";  
    std::string           entrust_type            = "";  
    std::string           support_trading_session = "";  
    std::string           time_in_force           = "";  
    std::string           side                    = "";  
    std::optional<double> quantity                = std::nullopt;  
    std::optional<double> limit_price             = std::nullopt;  
    std::optional<double> stop_price              = std::nullopt;  
};

std::string get_account_list(
          CURL*             curl,
    const Secret&           secret,
    const std::string_view& host,
    const std::string_view& token
);
std::string preview_order(
          CURL*             curl, 
    const Secret&           secret, 
    const std::string_view& host, 
    const std::string_view& token,
    const OrderRequest&     request);
std::string place_order(
          CURL*             curl, 
    const Secret&           secret, 
    const std::string_view& host, 
    const std::string_view& token,
    const OrderRequest&     request);
std::string modify_order(
          CURL*             curl, 
    const Secret&           secret, 
    const std::string_view& host, 
    const std::string_view& token,
    const OrderRequest&     request);
std::string cancel_order(
          CURL*             curl, 
    const Secret&           secret, 
    const std::string_view& host, 
    const std::string_view& token,
    const OrderRequest&     request);

}