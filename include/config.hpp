// config.hpp
#pragma once
#include <filesystem>
#include <map>
#include <string>

struct Config {
  std::string name;
  std::string license;
  std::filesystem::path templatePath;
  std::map<std::string, std::string> variables;
};