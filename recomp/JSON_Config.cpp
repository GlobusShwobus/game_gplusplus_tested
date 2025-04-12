#include "JSON_Config.h"

const nlohmann::json& JSONConfig::Get()const {
    return *config;
}

const nlohmann::json* JSONConfig::Get(const char* specific)const {

    if (config->contains(specific)) {
        return &(*config)[specific];
    }
    return nullptr;
}
