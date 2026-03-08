// validate_utils.cpp
#include "../include/validate_utils.hpp"
#include "../libs/localita/include/Localita.hpp"
#include <exception>
#include <filesystem>
#include <regex>
#include <vector>
#include <yaml-cpp/yaml.h> // public API kullan

namespace ValidateUtils {

bool ValidateTemplate(const YAML::Node &tmpl, std::vector<std::string> &errors,
                      Localita &loc) {
  // required fields
  if (!tmpl["name"])
    errors.push_back(loc.getText("MISSING_NAME"));
  if (!tmpl["version"])
    errors.push_back(loc.getText("MISSING_VERSION"));
  if (!tmpl["author"])
    errors.push_back(loc.getText("MISSING_AUTHOR"));
  if (!tmpl["description"])
    errors.push_back(loc.getText("MISSING_DESCRIPTION"));

  // --- variables (must be a map) ---
  YAML::Node vars = tmpl["variables"];
  if (vars) {
    if (!vars.IsMap()) {
      errors.push_back(loc.getText("VARIABLES_MUST_BE_MAP"));
    } else {
      static const std::regex key_re("^[a-zA-Z0-9_]+$");
      static const std::regex value_re("^\\{\\{[a-zA-Z0-9_]+\\}\\}$");

      for (auto it = vars.begin(); it != vars.end(); ++it) {
        YAML::Node keyNode = it->first;
        YAML::Node valNode = it->second;

        if (!keyNode.IsScalar()) {
          errors.push_back(loc.getText("VARIABLE_MUST_BE_SCALAR_STRING"));
          continue;
        }
        if (!valNode.IsScalar()) {
          errors.push_back(loc.getText("VARIABLE_VALUE_FOR_KEY") + '\'' +
                           keyNode.as<std::string>() + '\'' +
                           loc.getText("MUST_BE_SCALAR_STRING"));
          continue;
        }

        std::string key = keyNode.as<std::string>();
        std::string value = valNode.as<std::string>();

        if (!std::regex_match(key, key_re)) {
          errors.push_back(loc.getText("VARIABLE_KEY_INVALID") + key + "'");
        }
        if (!std::regex_match(value, value_re)) {
          errors.push_back(loc.getText("VARIABLE_VALUE_INVALID") + value + "'");
        }
      }
    }
  }

  // --- files (should be a sequence) ---
  YAML::Node files = tmpl["files"];
  if (files) {
    if (!files.IsSequence()) {
      errors.push_back(loc.getText("FILES_MUST_BE_SEQUENCE"));
    } else {
      static const std::regex path_re("^[a-zA-Z0-9_\\-\\/\\.]+$");

      for (std::size_t i = 0; i < files.size(); ++i) {
        YAML::Node f = files[i];

        if (!f["target"]) {
          errors.push_back(loc.getText("FILE_MISSING_TARGET") +
                           std::to_string(i));
        } else {
          std::string target = f["target"].as<std::string>();
          if (!std::regex_match(target, path_re)) {
            errors.push_back(loc.getText("FILE_TARGET_INVALID") +
                             std::to_string(i));
          }
        }

        if (!f["template"]) {
          errors.push_back(loc.getText("FILE_MISSING_TEMPLATE") +
                           std::to_string(i));
        } else {
          std::string template_path = f["template"].as<std::string>();
          if (!std::regex_match(template_path, path_re)) {
            errors.push_back(loc.getText("FILE_TEMPLATE_INVALID") +
                             std::to_string(i));
          }
        }
      }
    }
  }

  // --- command_mode (allowed: "cautious" or "execute_all") ---
  if (tmpl["command_mode"]) {
    try {
      std::string cm = tmpl["command_mode"].as<std::string>();
      if (cm != "cautious" && cm != "execute_all") {
        errors.push_back(loc.getText("COMMAND_MODE_INVALID"));
      }
    } catch (...) {
      errors.push_back(loc.getText("COMMAND_MODE_MUST_BE_STRING"));
    }
  }

  // --- silent_mode (must be boolean or boolean-like) ---
  if (tmpl["silent_mode"]) {
    try {
      bool sm = tmpl["silent_mode"].as<bool>();
      (void)sm;
    } catch (...) {
      errors.push_back(loc.getText("SILENT_MODE_INVALID"));
    }
  }

  // --- commands (must be sequence of maps with scalar "command") ---
  YAML::Node commands = tmpl["commands"];
  if (commands) {
    if (!commands.IsSequence()) {
      errors.push_back(loc.getText("COMMANDS_MUST_BE_SEQUENCE"));
    } else {
      for (std::size_t i = 0; i < commands.size(); ++i) {
        YAML::Node cmd_item = commands[i];
        if (!cmd_item.IsMap()) {
          errors.push_back(loc.getText("COMMANDS_ITEM_MUST_BE_MAP"));
          continue;
        }
        if (!cmd_item["command"] || !cmd_item["command"].IsScalar()) {
          errors.push_back(loc.getText("COMMANDS_ITEM_MISSING_COMMAND") +
                           std::to_string(i));
        }
      }
    }
  }

  // --- build_commands (must be sequence of maps with scalar "command") ---
  YAML::Node build_commands = tmpl["build_commands"];
  if (build_commands) {
    if (!build_commands.IsSequence()) {
      errors.push_back(loc.getText("BUILD_COMMANDS_MUST_BE_SEQUENCE"));
    } else {
      for (std::size_t i = 0; i < build_commands.size(); ++i) {
        YAML::Node cmd_item = build_commands[i];
        if (!cmd_item.IsMap()) {
          errors.push_back(loc.getText("BUILD_COMMANDS_ITEM_MUST_BE_MAP"));
          continue;
        }
        if (!cmd_item["command"] || !cmd_item["command"].IsScalar()) {
          errors.push_back(loc.getText("BUILD_COMMANDS_ITEM_MISSING_COMMAND") +
                           std::to_string(i));
        }
      }
    }
  }

  // --- run_commands (must be sequence of maps with scalar "command") ---
  YAML::Node run_commands = tmpl["run_commands"];
  if (run_commands) {
    if (!run_commands.IsSequence()) {
      errors.push_back(loc.getText("RUN_COMMANDS_MUST_BE_SEQUENCE"));
    } else {
      for (std::size_t i = 0; i < run_commands.size(); ++i) {
        YAML::Node cmd_item = run_commands[i];
        if (!cmd_item.IsMap()) {
          errors.push_back(loc.getText("RUN_COMMANDS_ITEM_MUST_BE_MAP"));
          continue;
        }
        if (!cmd_item["command"] || !cmd_item["command"].IsScalar()) {
          errors.push_back(loc.getText("RUN_COMMANDS_ITEM_MISSING_COMMAND") +
                           std::to_string(i));
        }
      }
    }
  }
  

  return errors.empty();
}

bool ValidateTemplate(const std::filesystem::path &path,
                      std::vector<std::string> &errors, Localita &loc) {
  try {
    YAML::Node tmpl = YAML::LoadFile(path.string());
    if (!ValidateTemplate(tmpl, errors, loc)) {
      return false;
    }
  } catch (const std::exception &e) {
    errors.emplace_back(e.what());
    return false;
  }
  return true;
}

} // namespace ValidateUtils
