#include "File_Manager.h"

const std::vector<std::filesystem::path> FileManager::GetTextureFolderContents(const char* relative_folder_path) const
{
    std::vector<std::filesystem::path> paths;

    for (const auto& entry : std::filesystem::directory_iterator(relative_folder_path)) {
        if (entry.path().extension() == ".png") {
            paths.push_back(entry);
        }
    }

    return paths;
}

const std::unique_ptr<nlohmann::json> FileManager::GetEntityConfig(const char* relative_path) const
{
    std::ifstream in(relative_path);

    if (in.good()) {
        auto ptr = std::make_unique<nlohmann::json>();
        in >> *ptr;
        return ptr;
    }

    return nullptr;
}
