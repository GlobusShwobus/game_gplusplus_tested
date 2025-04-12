#pragma once

#include <fstream>
#include <filesystem>
#include "json.hpp"

//previously was a class but decided the filemanager itself should never hold any data anyway so it got reduced down to namespace
namespace FileManager {

	const std::vector<std::filesystem::path> getPNGs(const char* folderPath);
	nlohmann::json* getNewConfig(const char* folderPath);

}
