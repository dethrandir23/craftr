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

std::vector<std::pair<std::filesystem::path, std::string>>
ListTemplates(const std::filesystem::path &path) {

  std::vector<std::pair<std::filesystem::path, std::string>> list;

  try {
    for (const auto &entry :
         std::filesystem::recursive_directory_iterator(path)) {
      if (!entry.is_regular_file())
        continue;
      const auto ext = entry.path().extension();
      if (ext == ".yml" || ext == ".yaml") {
        list.push_back({entry.path(), entry.path().stem().string()});
      }
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return list;
  }
  return list;
}

std::vector<std::pair<std::filesystem::path, std::string>>
ListTemplates(const std::vector<std::filesystem::path> &paths) {

  std::vector<std::pair<std::filesystem::path, std::string>> list;
  for (const auto &p : paths) {
    if (!std::filesystem::exists(p) || !std::filesystem::is_directory(p)) {
      continue;
    }
    auto sublist = ListTemplates(p);
    list.insert(list.end(), std::make_move_iterator(sublist.begin()),
                std::make_move_iterator(sublist.end()));
  }

  return list;
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

// std::vector<std::string> GetTemplateReplacerTypes(const std::string& yml);
// //
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

std::optional<std::string> GetTemplateName(const std::filesystem::path &path) {
  try {
    YAML::Node yaml = YAML::LoadFile(path.string());
    if (yaml["name"] && yaml["name"].IsScalar()) {
      return yaml["name"].as<std::string>();
    }
  } catch (const YAML::Exception &e) {
    std::cerr << "YAML error in " << path << ": " << e.what() << "\n";
  }
  return std::nullopt;
}

std::optional<std::string>
GetTemplateDescription(const std::filesystem::path &path) {
  try {
    YAML::Node yaml = YAML::LoadFile(path.string());
    if (yaml["description"] && yaml["description"].IsScalar()) {
      return yaml["description"].as<std::string>();
    }
  } catch (const YAML::Exception &e) {
    std::cerr << "YAML error in " << path << ": " << e.what() << "\n";
  }
  return std::nullopt;
}

std::optional<std::string>
GetTemplateAuthor(const std::filesystem::path &path) {
  try {
    YAML::Node yaml = YAML::LoadFile(path.string());
    if (yaml["author"] && yaml["author"].IsScalar()) {
      return yaml["author"].as<std::string>();
    }
  } catch (const YAML::Exception &e) {
    std::cerr << "YAML error in " << path << ": " << e.what() << "\n";
  }
  return std::nullopt;
}

std::optional<std::string>
GetTemplateVersion(const std::filesystem::path &path) {
  try {
    YAML::Node yaml = YAML::LoadFile(path.string());
    if (yaml["version"] && yaml["version"].IsScalar()) {
      return yaml["version"].as<std::string>();
    }
  } catch (const YAML::Exception &e) {
    std::cerr << "YAML error in " << path << ": " << e.what() << "\n";
  }
  return std::nullopt;
}

std::optional<TemplateMetadata>
GetTemplateMetadata(const std::filesystem::path &path) {
  try {
    YAML::Node yaml = YAML::LoadFile(path.string());
    TemplateMetadata metadata;
    metadata.path = path;

    if (yaml["name"] && yaml["name"].IsScalar()) {
      metadata.name = yaml["name"].as<std::string>();
    } else {
      metadata.name = path.stem().string();
    }

    if (yaml["description"] && yaml["description"].IsScalar()) {
      metadata.description = yaml["description"].as<std::string>();
    }

    if (yaml["author"] && yaml["author"].IsScalar()) {
      metadata.author = yaml["author"].as<std::string>();
    }

    if (yaml["version"] && yaml["version"].IsScalar()) {
      metadata.version = yaml["version"].as<std::string>();
    }

    return metadata;
  } catch (const YAML::Exception &e) {
    std::cerr << "YAML error in " << path << ": " << e.what() << "\n";
  }
  return std::nullopt;
}
} // namespace TemplateUtils