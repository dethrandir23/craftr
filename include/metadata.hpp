#pragma once

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

namespace {
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
}

namespace Metadata {
inline nlohmann::json load() {
  std::ifstream file(getDataDir("craftr").append("metadata/metadata.json"));
  if (!file.is_open()) {
    throw std::runtime_error("Could not open metadata.json");
  }
  nlohmann::json metadata;
  file >> metadata;
  return metadata;
}

inline std::string name() { return load().value("name", "UnknownProject"); }

inline std::string version() { return load().value("version", "0.0.0"); }

inline std::string author() { return load().value("author", "UnknownAuthor"); }
} // namespace Metadata
