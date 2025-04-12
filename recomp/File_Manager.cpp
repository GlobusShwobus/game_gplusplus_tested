#include "File_Manager.h"

const std::vector<std::filesystem::path> FileManager::getPNGs(const char* folderPath)
{
    std::vector<std::filesystem::path> paths;

    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.path().extension() == ".png") {
            paths.push_back(entry);
        }
    }

    return paths;
}

nlohmann::json* FileManager::getNewConfig(const char* folderPath)
{
    std::ifstream in(folderPath);
    nlohmann::json* config = nullptr;
   
    if (in.good()) {
        config = new nlohmann::json();
        in >> *config;
    }

    return config;
}
