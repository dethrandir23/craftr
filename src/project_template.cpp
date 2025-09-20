// project_template.cpp

#include "../include/project_template.hpp"
#include <iostream>
#include <string>
#include <utility>
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

    if (config["variables"]) {
      for (auto it : config["variables"]) {
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
        } else if (it["template"].IsMap()) { // TODO : Refactor later, we not
                                             // use license map system anymore.
          for (auto lic : it["template"]) {
            std::string license_key = lic.first.as<std::string>();
            std::string tmpl_path = lic.second.as<std::string>();

            licenses[license_key] = tmpl_path;
          }
        }
      }
    }

    if (config["command_mode"]) {
      if (config["command_mode"].as<std::string>() == "cautious") {
        commandMode = CommandUtils::CommandMode::Cautious;
      } else if (config["command_mode"].as<std::string>() == "execute_all") {
        commandMode = CommandUtils::CommandMode::ExecuteAll;
      } else {
        std::cerr << "Unknown command mode in your template file. please set "
                     "it to \"cautious\" or \"execute_all\" "
                  << std::endl;
        std::cerr << "Craftr sets command mode to cautious now." << std::endl;
        commandMode = CommandUtils::CommandMode::Cautious;
      }
    }

    if (config["silent_mode"]) {
      if (config["silent_mode"].as<std::string>() == "true") {
        silent_mode = true;
      } else if (config["silent_mode"].as<std::string>() == "false") {
        silent_mode = false;
      } else {
        silent_mode = false;
      }
    } else {
      silent_mode = false;
    }

    if (config["commands"]) {
      for (auto it : config["commands"]) {
        if (it["command"]) {
          if (it["description"]) {
            commands.emplace_back(
                std::make_pair(it["command"].as<std::string>(),
                               it["description"].as<std::string>()));
          } else {
            commands.emplace_back(
                std::make_pair(it["command"].as<std::string>(), ""));
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