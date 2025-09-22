#pragma once

#include "paths.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#if defined(_WIN32)
#include <windows.h>
#endif

namespace Metadata {
inline nlohmann::json load() {
  std::ifstream file(Paths::getDataDir("craftr").append("metadata/metadata.json"));
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
