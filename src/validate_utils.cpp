// validate_utils.cpp
#include "../include/validate_utils.hpp"
#include <exception>
#include <filesystem>
#include <regex>
#include <vector>
#include <yaml-cpp/yaml.h> // public API kullan

namespace ValidateUtils {

bool ValidateTemplate(const YAML::Node &tmpl,
                      std::vector<std::string> &errors) {
  // required fields
  if (!tmpl["name"])
    errors.push_back("Missing required field: name");
  if (!tmpl["version"])
    errors.push_back("Missing required field: version");
  if (!tmpl["author"])
    errors.push_back("Missing required field: author");
  if (!tmpl["description"])
    errors.push_back("Missing required field: description");

  // --- variables (must be a map) ---
  YAML::Node vars = tmpl["variables"];
  if (vars) {
    if (!vars.IsMap()) {
      errors.push_back("Field 'variables' must be a map of key-value pairs.");
    } else {
      static const std::regex key_re("^[a-zA-Z0-9_]+$");
      static const std::regex value_re("^\\{\\{[a-zA-Z0-9_]+\\}\\}$");

      for (auto it = vars.begin(); it != vars.end(); ++it) {
        YAML::Node keyNode = it->first;
        YAML::Node valNode = it->second;

        if (!keyNode.IsScalar()) {
          errors.push_back("Variable key must be a scalar string.");
          continue;
        }
        if (!valNode.IsScalar()) {
          errors.push_back("Variable value for key '" +
                           keyNode.as<std::string>() +
                           "' must be a scalar string.");
          continue;
        }

        std::string key = keyNode.as<std::string>();
        std::string value = valNode.as<std::string>();

        if (!std::regex_match(key, key_re)) {
          errors.push_back(
              "Variable key '" + key +
              "' can only contain letters, numbers, or underscores.");
        }
        if (!std::regex_match(value, value_re)) {
          errors.push_back("Variable value '" + value +
                           "' must be in the format '{{VARIABLE_NAME}}'.");
        }
      }
    }
  }

  // --- files (should be a sequence) ---
  YAML::Node files = tmpl["files"];
  if (files) {
    if (!files.IsSequence()) {
      errors.push_back("Field 'files' must be a sequence.");
    } else {
      static const std::regex path_re("^[a-zA-Z0-9_\\-\\/\\.]+$");

      for (std::size_t i = 0; i < files.size(); ++i) {
        YAML::Node f = files[i];

        if (!f["target"]) {
          errors.push_back("files[" + std::to_string(i) +
                           "] is missing 'target'");
        } else {
          std::string target = f["target"].as<std::string>();
          if (!std::regex_match(target, path_re)) {
            errors.push_back("files[" + std::to_string(i) +
                             "] 'target' must be a valid file path.");
          }
        }

        if (!f["template"]) {
          errors.push_back("files[" + std::to_string(i) +
                           "] is missing 'template'");
        } else {
          std::string template_path = f["template"].as<std::string>();
          if (!std::regex_match(template_path, path_re)) {
            errors.push_back("files[" + std::to_string(i) +
                             "] 'template' must be a valid file path.");
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
        errors.push_back(
            "command_mode can only be \"cautious\" or \"execute_all\"");
      }
    } catch (...) {
      errors.push_back("command_mode must be a string.");
    }
  }

  // --- silent_mode (must be boolean or boolean-like) ---
  if (tmpl["silent_mode"]) {
    // prefer boolean, accept "true"/"false" too via as<bool>()
    try {
      bool sm = tmpl["silent_mode"].as<bool>();
      (void)sm; // check as as<bool>() if even not used
    } catch (...) {
      errors.push_back("silent_mode must be boolean (true/false).");
    }
  }

  YAML::Node commands = tmpl["commands"];
  if (commands) {
    if (!commands.IsSequence()) {
      errors.push_back("Field 'commands' must be a sequence.");
    } else {
      for (std::size_t i = 0; i < commands.size(); ++i) {
        YAML::Node cmd_item = commands[i];
        if (!cmd_item.IsMap()) {
          errors.push_back("Each item in 'commands' must be a map.");
          continue;
        }
        if (!cmd_item["command"] || !cmd_item["command"].IsScalar()) {
          errors.push_back("commands[" + std::to_string(i) +
                           "] is missing a scalar 'command' field.");
        }
      }
    }
  }

  return errors.empty();
}

bool ValidateTemplate(const std::filesystem::path &path,
                      std::vector<std::string> &errors) {
  try {
    YAML::Node tmpl = YAML::LoadFile(path.string());
    if (!ValidateTemplate(tmpl, errors)) {
      return false;
    }
  } catch (const std::exception &e) {
    errors.emplace_back(e.what());
    return false;
  }
  return true;
}

} // namespace ValidateUtils
