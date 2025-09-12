//file_utils.hpp
#pragma once

#include "content_file.hpp"
#include <filesystem>
#include <string>
#include <vector>

namespace FileUtils
{
    bool create_file(const std::filesystem::path& path, const std::string& filename, const std::string& content);
    std::filesystem::path get_project_subfolder(const std::string& project_name);
    std::string LoadFileToString(const std::string& filepath);
    std::filesystem::path get_project_root();
    std::filesystem::path get_templates_folder();
    bool write_content_files(const std::vector<ContentFile>& files);
    bool create_subfolders(const std::vector<std::filesystem::path>& subfolders, const std::filesystem::path& basePath);

} //namespace FileUtils