//file_utils.cpp
#include "../include/file_utils.hpp"
#include <filesystem>
#include <string>
#include <fstream>

namespace FileUtils
{
    bool create_file(const std::filesystem::path& path, 
                     const std::string& filename, 
                     const std::string& content) 
    {
        try {
            std::filesystem::path fullPath = path / filename;
            std::ofstream file(fullPath);
            if (!file.is_open()) {
                return false;
            }   
            file << content;
        } catch (...) {
            return false;
        }

        return true;
    }

    std::filesystem::path get_project_subfolder(const std::string& project_name) {
        std::filesystem::path currentDir = std::filesystem::current_path();
        return currentDir / project_name;
    }
} // namespace FileUtils
