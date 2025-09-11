//file_utils.hpp
#pragma once

#include <filesystem>
#include <string>

namespace FileUtils
{
    bool create_file(const std::filesystem::path& path, const std::string& filename, const std::string& content);
    std::filesystem::path get_project_subfolder(const std::string& project_name);
    std::string LoadFileToString(const std::string& filepath);
    std::filesystem::path get_project_root();
    std::filesystem::path get_templates_folder();
} //namespace FileUtils