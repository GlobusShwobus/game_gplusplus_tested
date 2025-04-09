#pragma once

#include "File_Manager.h"

class JSONConfig {
    std::unique_ptr<nlohmann::json> config;

public:
    JSONConfig() = default;

    void Init(std::unique_ptr<nlohmann::json> config);
    const nlohmann::json& Get()const;
    //kinda shitty atm, need to work with enums if i plan on just having a single big dick config file
    const nlohmann::json* Get(const char* specific)const {


        if (config && config->contains(specific)) {
            return &(*config)[specific];
        }
        return nullptr;

    }

    bool Good()const;
};