#pragma once

#include "File_Manager.h"

class JSONConfig {
    nlohmann::json* config = nullptr;

public:
    JSONConfig() = delete;
    JSONConfig(nlohmann::json* takeOwnership) {
        if (takeOwnership == nullptr) {
            throw std::exception("Recieved nullptr initalizing config recieved by FileManager (wrong path?)");
        }
        config = takeOwnership;
    }

    const nlohmann::json& Get()const;
    //can return nullptr
    const nlohmann::json* Get(const char* specific)const;
public:

    JSONConfig(const JSONConfig& copy) = delete;
    JSONConfig(JSONConfig&& move)noexcept = delete;
    JSONConfig& operator=(const JSONConfig& assign) = delete;
    JSONConfig& operator=(JSONConfig&& assign)noexcept = delete;

    ~JSONConfig() {
        if (config) {
            delete config;
            config = nullptr;
        }
    }
};