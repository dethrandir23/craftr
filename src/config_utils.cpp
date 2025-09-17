// config_utils.cpp

#include "../include/config_utils.hpp"
#include <optional>
#include <string>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>

namespace ConfigUtils {

std::optional<std::filesystem::path>
FindConfigByName(const std::filesystem::path &directory,
                 const std::string &name) {
  for (const auto &entry : std::filesystem::directory_iterator(directory)) {
    if (entry.is_regular_file()) {
      if (entry.path().extension() == ".yml" ||
          entry.path().extension() == ".yaml") {
        YAML::Node config = YAML::LoadFile(entry.path().string());
        if (config["name"] && config["name"].as<std::string>() == name) {
          return entry.path();
        }
      }
    }
  }
  return std::nullopt;
}

Config LoadConfig(const YAML::Node &ymlFile) {
  Config config;

  if (ymlFile["name"]) {
    config.name = ymlFile["name"].as<std::string>();
  } else {
    config.name = "unknown";
  }

  if (ymlFile["variables"]) {
    for (auto it : ymlFile["variables"]) {
      config.variables[it.first.as<std::string>()] =
          it.second.as<std::string>();
    }
  }
  return config;
}

Config LoadConfig(const std::filesystem::path &path) {
  return LoadConfig(YAML::LoadFile(path.string()));
}

Config LoadConfig(const std::filesystem::path &directory,
                  const std::string &name) {
  return LoadConfig(FindConfigByName(directory, name).value());
}

std::vector<std::string> GetVariableNames(const YAML::Node &ymlFile) {
  std::vector<std::string> variable_names;
  if (ymlFile["variables"]) {
    for (auto it : ymlFile["variables"]) {
      variable_names.emplace_back(it.first.as<std::string>());
    }
  }
  return variable_names;
}
std::vector<std::string> GetVariableNames(const std::filesystem::path &path) {
  return GetVariableNames(YAML::LoadFile(path));
}

std::vector<std::string>
GetVariableNames(const std::filesystem::path &directory,
                 const std::string &name) {
  return GetVariableNames(FindConfigByName(directory, name).value());
}

} // namespace ConfigUtils