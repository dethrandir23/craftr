// project_template.cpp

#include "../include/project_template.hpp"
#include <iostream>
#include <string>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/yaml.h>

bool ProjectTemplate::LoadTemplate() {
  try {
    YAML::Node config = YAML::LoadFile(template_path.string());
    if (config["name"])
      name = config["name"].as<std::string>();
    if (config["version"])
      version = config["version"].as<std::string>();
    if (config["author"])
      author = config["author"].as<std::string>();
    if (config["description"])
      description = config["description"].as<std::string>();

    if (config["replacers"]) {
      for (auto it : config["replacers"]) {
        replacers.emplace_back(it.first.as<std::string>(),
                               it.second.as<std::string>());
      }
    }

    if (config["subfolders"]) {
      for (auto it : config["subfolders"]) {
        subfolders.push_back(it.as<std::string>());
      }
    }

    if (config["files"]) {
      for (auto it : config["files"]) {
        std::string target = it["target"].as<std::string>();

        if (it["template"].IsScalar()) {
          std::string tmpl = it["template"].as<std::string>();
          file_blueprints.emplace_back(FileBlueprint(tmpl, target));
        } else if (it["template"].IsMap()) {
          for (auto lic : it["template"]) {
            std::string license_key = lic.first.as<std::string>();
            std::string tmpl_path = lic.second.as<std::string>();

            licenses[license_key] = tmpl_path;
          }
        }
      }
    }

    return true;

  } catch (const std::exception &e) {
    std::cerr << "YAML parse error: " << e.what() << "\n";
    return false;
  }

  return true;
}