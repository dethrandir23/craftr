// links.hpp
#pragma once

#include "paths.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#if defined(_WIN32)
#include <windows.h>
#endif

namespace Links {
inline nlohmann::json load() {
  std::ifstream file(Paths::getDataDir("craftr").append("links/links.json"));
  if (!file.is_open()) {
    throw std::runtime_error("Could not open links.json");
  }
  nlohmann::json metadata;
  file >> metadata;
  return metadata;
}

inline std::string main_templates_repo () { return load().value("main_templates_repo", "error"); }

inline std::string main_licenses_repo () { return load().value("main_licenses_repo", "error"); }

} // namespace Links
