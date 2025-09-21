// file_utils.hpp
#pragma once

#include "content_file.hpp"
#include "project.hpp"
#include <filesystem>
#include <string>
#include <vector>

namespace FileUtils {
bool create_file(const std::filesystem::path &path, const std::string &filename,
                 const std::string &content);
std::filesystem::path get_project_subfolder(const std::string &project_name);
std::string LoadFileToString(const std::string &filepath);
std::filesystem::path get_templates_folder();
bool write_content_files(const std::vector<ContentFile> &files);
bool create_subfolders(const std::vector<std::filesystem::path> &subfolders,
                       const std::filesystem::path &basePath);

bool write_project(const Project &project);

std::vector<std::string>
getFileNames(const std::filesystem::path &directoryPath);
std::vector<std::filesystem::path>
getFilePaths(const std::filesystem::path &directoryPath);
std::map<std::string, std::filesystem::path>
getFileMap(const std::filesystem::path &directoryPath);

std::filesystem::path getConfigDir(const std::string& app_name);
std::filesystem::path getDataDir(const std::string &app_name);

} // namespace FileUtils