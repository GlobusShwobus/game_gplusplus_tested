#include "JSON_Config.h"

void JSONConfig::Init(std::unique_ptr<nlohmann::json> _config) {
    config = std::move(_config);
}

const nlohmann::json& JSONConfig::Get()const {
    return *config;
}

bool JSONConfig::Good()const {
    return config != nullptr;
}