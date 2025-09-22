// template_utils.cpp
#include "../include/template_utils.hpp"
#include "../include/project_template.hpp"
#include <filesystem>
#include <iostream>
#include <optional>
#include <vector>
#include <yaml-cpp/yaml.h>

namespace TemplateUtils {
std::optional<std::filesystem::path>
FindTemplateByName(const std::filesystem::path &directory,
                   const std::string &name) {
  try {
    for (const auto &entry : std::filesystem::directory_iterator(directory)) {
      try {
        if (entry.is_regular_file()) {
          const auto ext = entry.path().extension();
          if (ext == ".yml" || ext == ".yaml") {
            YAML::Node tmp = YAML::LoadFile(entry.path().string());
            if (tmp["name"] && tmp["name"].as<std::string>() == name) {
              return entry.path();
            }
          }
        }
      } catch (const YAML::Exception &e) {
        std::cerr << "YAML error in " << entry.path() << ": " << e.what()
                  << "\n";
        continue;
      }
    }
  } catch (const std::filesystem::filesystem_error &e) {
    std::cerr << "Directory error: " << e.what() << "\n";
  }
  return std::nullopt;
}

std::vector<std::filesystem::path>
FindTemplatesByName(const std::vector<std::filesystem::path> &directories,
                    const std::string &name) {
  std::vector<std::filesystem::path> results;

  for (const auto &directory : directories) {
    if (!std::filesystem::exists(directory) ||
        !std::filesystem::is_directory(directory)) {
      continue;
    }

    try {
      for (const auto &entry :
           std::filesystem::recursive_directory_iterator(directory)) {
        try {
          if (!entry.is_regular_file())
            continue;

          const auto ext = entry.path().extension();
          if (ext == ".yml" || ext == ".yaml") {
            YAML::Node tmp = YAML::LoadFile(entry.path().string());
            if (tmp["name"] && tmp["name"].as<std::string>() == name) {
              results.push_back(entry.path());
            }
          }
        } catch (const YAML::Exception &e) {
          std::cerr << "YAML error in " << entry.path() << ": " << e.what()
                    << "\n";
          continue;
        }
      }
    } catch (const std::filesystem::filesystem_error &e) {
      std::cerr << "Directory iteration error for " << directory << ": "
                << e.what() << "\n";
      continue;
    }
  }

  return results;
}

// ProjectTemplate LoadProjectTemplate(const xYAML::Node &yml) { return; } //
// TODO implement
ProjectTemplate LoadProjectTemplate(const std::filesystem::path &path) {
  ProjectTemplate ptmp(path);
  ptmp.LoadTemplate();
  return ptmp;
}
ProjectTemplate LoadProjectTemplate(const std::filesystem::path &directory,
                                    const std::string &name) {
  return LoadProjectTemplate(FindTemplateByName(directory, name).value());
}

// std::vector<std::string> GetTemplateReplacerTypes(const std::string& yml); //
// TODO implement
std::vector<std::string> GetTemplateReplacerTypes(const std::string &path) {
  std::vector<std::string> types;
  ProjectTemplate ptmp(path);
  ptmp.LoadTemplate();
  auto replacers = ptmp.GetReplacers();
  for (auto r : replacers) {
    types.emplace_back(r.GetType());
  }
  return types;
}
std::vector<std::string>
GetTemplateReplacerTypes(const std::filesystem::path &directory,
                         const std::string &name) {
  return GetTemplateReplacerTypes(FindTemplateByName(directory, name).value());
}
} // namespace TemplateUtils