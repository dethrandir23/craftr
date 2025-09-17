// config_utils.hpp
#pragma once

#include "config.hpp"
#include <filesystem>
#include <optional>
#include <string>
#include <vector>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/yaml.h>

namespace ConfigUtils {

std::optional<std::filesystem::path>
FindConfigByName(const std::filesystem::path &directory,
                 const std::string &name);

Config LoadConfig(const YAML::Node &ymlFile);
Config LoadConfig(const std::filesystem::path &path);
Config LoadConfig(const std::filesystem::path &directory, const std::string &name);

std::vector<std::string> GetVariableNames(const YAML::Node &ymlFile);
std::vector<std::string> GetVariableNames(const std::filesystem::path &path);
std::vector<std::string>
GetVariableNames(const std::filesystem::path &directory,
                 const std::string &name);
} // namespace ConfigUtils