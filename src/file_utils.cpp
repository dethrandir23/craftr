// file_utils.cpp
#include "../include/file_utils.hpp"
#include "../include/project.hpp"
#include "../include/metadata.hpp"
#include "../include/string_utils.hpp"
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#if defined(_WIN32)
#include <windows.h>
#endif

namespace FileUtils {
bool create_file(const std::filesystem::path &path, const std::string &filename,
                 const std::string &content) {
  try {
    if (!path.empty() && !std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    std::filesystem::path fullPath = path / filename;
    std::ofstream file(fullPath);
    if (!file.is_open()) {
      std::cerr << "Error: Could not open file for writing: " << fullPath
                << std::endl;
      return false;
    }
    file << content;
    file.close();
  } catch (const std::exception &e) {
    std::cerr << "Error creating file: " << e.what() << std::endl;
    return false;
  }

  return true;
}

std::filesystem::path get_project_subfolder(const std::string &project_name) {
  std::filesystem::path currentDir = std::filesystem::current_path();
  return currentDir / project_name;
}

std::string LoadFileToString(const std::string &filepath) {
  std::ifstream file(filepath);
  if (!file.is_open()) {
    throw std::runtime_error("Can't open file: " + filepath);
  }

  std::ostringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

bool write_content_files(const std::vector<ContentFile> &files,
                         const std::filesystem::path &basePath) {
  try {
    for (const auto &file_to_create : files) {
      std::filesystem::path target = basePath / file_to_create.target_path;
      std::filesystem::path directory = target.parent_path();
      std::string filename = target.filename().string();

      if (!create_file(directory, filename,
                       file_to_create.content.get_text())) {
        std::cerr << "Failed to create one or more files. Aborting."
                  << std::endl;
        return false;
      }
    }
  } catch (const std::exception &e) {
    std::cerr << "An error occurred while writing files: " << e.what()
              << std::endl;
    return false;
  }
  return true;
}

bool create_subfolders(const std::vector<std::filesystem::path> &subfolders,
                       const std::filesystem::path &basePath) {
  try {
    for (const auto &folder : subfolders) {
      std::filesystem::path fullPath = basePath / folder;
      if (!std::filesystem::exists(fullPath)) {
        std::filesystem::create_directories(fullPath);
      }
    }
  } catch (const std::exception &e) {
    std::cerr << "Error creating subfolders: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool write_project(const Project &project) {
  try {
    if (!create_subfolders(project.getSubFolders(),
                           project.getProjectSubFolder())) {
      return false;
    }
    if (!write_content_files(project.getContentFiles(),
                             project.getProjectSubFolder())) {
      return false;
    }

  } catch (const std::exception &e) {
    std::cerr << "An error occurred while writing files: " << e.what()
              << std::endl;
    return false;
  }
  return true;
}

std::vector<std::string>
getFileNames(const std::filesystem::path &directoryPath) {
  std::vector<std::string> fileNames;
  for (const auto &entry : std::filesystem::directory_iterator(directoryPath)) {
    if (entry.is_regular_file()) {
      fileNames.push_back(StringUtils::toLower(entry.path().stem().string()));
    }
  }
  return fileNames;
}

std::vector<std::filesystem::path>
getFilePaths(const std::filesystem::path &directoryPath) {
  std::vector<std::filesystem::path> paths;
  for (const auto &entry : std::filesystem::directory_iterator(directoryPath)) {
    if (entry.is_regular_file()) {
      paths.push_back(entry.path());
    }
  }
  return paths;
}

std::map<std::string, std::filesystem::path>
getFileMap(const std::filesystem::path &directoryPath) {
  std::map<std::string, std::filesystem::path> fileMap;
  for (const auto &entry : std::filesystem::directory_iterator(directoryPath)) {
    if (entry.is_regular_file()) {
      fileMap[StringUtils::toLower(entry.path().stem().string())] =
          entry.path();
    } 
  }
  return fileMap;
}

std::filesystem::path getConfigDir(const std::string &app_name) {
  std::filesystem::path config_path;

#if defined(_WIN32)
  const char *appdata = std::getenv("APPDATA");
  if (appdata != nullptr) {
    config_path = std::filesystem::path(appdata) / app_name;
  } else {
    const char *userprofile = std::getenv("USERPROFILE");
    if (userprofile != nullptr) {
      config_path = std::filesystem::path(userprofile) / app_name;
    } else {
      config_path = std::filesystem::current_path() / app_name;
    }
  }
#elif defined(__APPLE__)
  const char *home = std::getenv("HOME");
  if (home != nullptr) {
    config_path = std::filesystem::path(home) / "Library" /
                  "Application Support" / app_name;
  } else {
    config_path = std::filesystem::current_path() / app_name;
  }
#elif defined(__linux__)
  const char *xdg_config_home = std::getenv("XDG_CONFIG_HOME");
  if (xdg_config_home != nullptr && std::string(xdg_config_home).length() > 0) {
    config_path = std::filesystem::path(xdg_config_home) / app_name;
  } else {
    const char *home = std::getenv("HOME");
    if (home != nullptr) {
      config_path = std::filesystem::path(home) / ".config" / app_name;
    } else {
      config_path = std::filesystem::current_path() / app_name;
    }
  }
#else
#error "Unsupported operating system!"
#endif

  try {
    if (!std::filesystem::exists(config_path)) {
      std::filesystem::create_directories(config_path);
    }
  } catch (const std::filesystem::filesystem_error &e) {
    std::cerr << "Failed to create configuration directory: " << e.what()
              << std::endl;
    return std::filesystem::path();
  }

  return config_path;
}

std::filesystem::path getDataDir(const std::string &app_name) {
  std::filesystem::path data_path;

#if defined(_WIN32)
  const char *local_appdata = std::getenv("LOCALAPPDATA");
  if (local_appdata != nullptr) {
    data_path = std::filesystem::path(local_appdata) / app_name;
  }
#elif defined(__APPLE__)
  const char *home = std::getenv("HOME");
  if (home != nullptr) {
    data_path = std::filesystem::path(home) / "Library" /
                "Application Support" / app_name;
  }
#elif defined(__linux__)
  const char *xdg_data_home = std::getenv("XDG_DATA_HOME");
  if (xdg_data_home != nullptr && std::string(xdg_data_home).length() > 0) {
    data_path = std::filesystem::path(xdg_data_home) / app_name;
  } else {
    const char *home = std::getenv("HOME");
    if (home != nullptr) {
      data_path = std::filesystem::path(home) / ".local" / "share" / app_name;
    }
  }
#else
#error "Unsupported Operating System!"
#endif

  if (data_path.empty()) {
    data_path = std::filesystem::current_path() / app_name / "data";
  }

  try {
    std::filesystem::create_directories(data_path);
  } catch (const std::filesystem::filesystem_error &e) {
    std::cerr << "Failed to create data directory: " << e.what() << std::endl;
    return std::filesystem::path();
  }

  return data_path;
}

std::filesystem::path get_templates_folder() {
    std::string app_name = StringUtils::toLower(Metadata::name());
    auto user_dir = Paths::getDataDir(app_name) / "templates";
    auto system_dir = Paths::getSystemDataDir() / "templates";
    
    if (!std::filesystem::exists(user_dir)) {
        if (std::filesystem::exists(system_dir)) {
            try {
                std::filesystem::create_directories(user_dir);
                for (const auto& entry : std::filesystem::recursive_directory_iterator(system_dir)) {
                    if (entry.is_regular_file()) {
                        auto relative_path = std::filesystem::relative(entry.path(), system_dir);
                        auto target_path = user_dir / relative_path;
                        std::filesystem::create_directories(target_path.parent_path());
                        std::filesystem::copy_file(entry.path(), target_path, 
                                                 std::filesystem::copy_options::overwrite_existing);
                    }
                }
            } catch (const std::filesystem::filesystem_error& e) {
                std::cerr << "Failed to copy templates: " << e.what() << std::endl;
                return system_dir; // Fallback to system directory
            }
        } else {
            return user_dir;
        }
    }
    
    return user_dir;
}

} // namespace FileUtils
