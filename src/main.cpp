/**
 * @file        main.cpp
 * @author      dethrandir23 (efeoztrk23@proton.me)
 * @brief       Entry point for the craftr CLI tool.
 * @version     1.0.0
 * @date        2025-09-04
 *
 * @copyright   Copyright (c) 2025
 *
 * @details
 * This file launches the craftr command-line interface, which helps developers
 * create and manage C++ projects using predefined templates. It uses the
 * Cliopatra library to parse command-line arguments and delegates tasks such
 * as:
 *   - Creating a new project (`--create`)
 *   - Adding modules or components (`--add`)
 *   - Building the project (`--build`)
 *   - Specifying project metadata (author and license)
 *
 * It also provides utility commands like `--help` and `--version`.
 */

#include "../include/Cliopatra.hpp"
#include "../include/config.hpp"
#include "../include/extractor.hpp"
#include "../include/file_blueprint.hpp"
#include "../include/file_utils.hpp"
#include "../include/git_utils.hpp"
#include "../include/license_utils.hpp"
#include "../include/links.hpp"
#include "../include/locale_utils.hpp"
#include "../include/metadata.hpp"
#include "../include/output_utils.hpp"
#include "../include/project_utils.hpp"
#include "../include/string_utils.hpp"
#include "../include/template_utils.hpp"
#include "../include/validate_utils.hpp"
#include "../libs/localita/include/Localita.hpp"
#include <exception>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

// ---------------- Helper Functions ----------------

bool ensureTemplateProvided(const Cliopatra::ParsedMap &results,
                            Localita &loc) {
  if (results.find("template") == results.end()) {
    std::cerr << "[" << loc.getText("ERROR") << "] "
              << loc.getText("ERROR_TEMPLATE_NOT_SPECIFIED") << std::endl;
    return false;
  }
  return true;
}

int handleValidate(
    const std::vector<std::filesystem::path> &templates_directories,
    const std::string &templateName, Localita &loc) {

  auto templates =
      TemplateUtils::FindTemplatesByName(templates_directories, templateName);

  if (templates.empty()) {
    std::cerr << "[" << loc.getText("ERROR") << "] "
              << loc.getText("TEMPLATE_NOT_FOUND") << templateName << std::endl;
    return 1;
  }

  std::filesystem::path templateToValidate;
  if (templates.size() > 1) {
    std::cout << loc.getText("MULTIPLE_TEMPLATES_FOUND") << std::endl;
    for (size_t i = 0; i < templates.size(); ++i) {
      std::cout << i + 1 << ". " << templates[i] << "\n";
    }
    std::cout << loc.getText("WHICH_ONE_TO_VALIDATE") << " [1-"
              << templates.size() << "]: ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    templateToValidate = templates[choice - 1];
  } else {
    templateToValidate = templates.front();
  }

  std::vector<std::string> errors;
  ValidateUtils::ValidateTemplate(templateToValidate, errors, loc);

  if (errors.empty()) {
    std::cout << "[" << loc.getText("OK") << "] " << loc.getText("TEMPLATE")
              << " '" << templateName << "' (" << templateToValidate.string()
              << ")" << loc.getText("IS_VALID") << std::endl;
  } else {
    std::cout << "[" << loc.getText("FAIL") << "] "
              << "Validation errors for template '" << templateName << "' ("
              << templateToValidate.string() << "):" << std::endl;
    for (const auto &e : errors) {
      std::cout << "  - " << e << std::endl;
    }
    return 1;
  }

  return 0;
}
int handleCreate(
    const std::vector<std::filesystem::path> &templates_directories,
    const std::string &templateName, Localita &loc) {
  Config config;

  auto templates =
      TemplateUtils::FindTemplatesByName(templates_directories, templateName);

  if (templates.empty()) {
    std::cerr << "[" << loc.getText("ERROR") << "] "
              << loc.getText("TEMPLATE_NOT_FOUND") << templateName << std::endl;
    return 1;
  }
  config.templatePath = templates.front();
  if (templates.size() > 1) {
    std::cout << loc.getText("MULTIPLE_TEMPLATES_FOUND") << std::endl;
    for (size_t i = 0; i < templates.size(); ++i) {
      std::cout << i + 1 << ". " << templates[i] << "\n";
    }
    std::cout << loc.getText("SELECT_ONE") << " [1-" << templates.size()
              << "]: ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    config.templatePath = templates[choice - 1];
  }

  for (auto key :
       TemplateUtils::GetTemplateReplacerTypes(config.templatePath)) {
    if (key == "DATE") {
      config.variables[key] = DateUtils::GetCurrentYearStr();
      continue;
    }
    std::string input;
    std::cout << loc.getText("ENTER") << key << ": ";
    std::getline(std::cin, input);
    config.variables[key] = input;
    if (key == "PROJECT_NAME") {
      config.name = StringUtils::trim(input);
    }
  }

  std::cout << loc.getText("LICENSE_TYPE_SELECTION") << std::endl;
  for (auto l : LicenseUtils::GetLicenseTypes()) {
    std::cout << "- " << l << "\n";
  }
  std::cout << loc.getText("SELECTED_LICENSE");
  std::string input;
  std::getline(std::cin, input);
  config.license =
      StringUtils::trim(input).empty() ? "none" : StringUtils::trim(input);

  ProjectUtils::create_project(config, loc);
  return 0;
}

int handleLanguage(const std::string &newLocale, Localita &loc) {
  auto oldLoc = LocaleUtils::getPreferredLocale(loc);

  auto availableLocales = LocaleUtils::getAvailableLocales(loc);
  bool isValidLocale =
      std::find(availableLocales.begin(), availableLocales.end(), newLocale) !=
      availableLocales.end();

  if (newLocale == "system") {
    if (LocaleUtils::setConfigLocale("system")) {
      auto systemLocale = LocaleUtils::getSystemLocale();
      std::cout << "[" << loc.getText("OK") << "] "
                << loc.getText("LANGUAGE_SET_TO_SYSTEM") << " (" << systemLocale
                << ")" << std::endl;
      return 0;
    } else {
      std::cerr << "[" << loc.getText("ERROR") << "] "
                << loc.getText("FAILED_TO_SET_LANGUAGE") << std::endl;
      return 1;
    }
  }

  if (isValidLocale) {
    if (LocaleUtils::setConfigLocale(newLocale)) {
      std::cout << "[" << loc.getText("OK") << "] "
                << loc.getText("LANGUAGE_SET_TO") << " '" << newLocale << "'"
                << std::endl;
      return 0;
    } else {
      std::cerr << "[" << loc.getText("ERROR") << "] "
                << loc.getText("FAILED_TO_SET_LANGUAGE") << std::endl;
      return 1;
    }
  }

  std::cerr << "[" << loc.getText("ERROR") << "] "
            << loc.getText("SELECTED_LANGUAGE_NOT_EXISTS") << std::endl;
  std::cerr << loc.getText("AVAILABLE_LANGUAGES") << std::endl;

  for (const auto &locale : availableLocales) {
    std::cerr << "- " << locale << std::endl;
  }

  std::cerr << loc.getText("OR_USE_SYSTEM_LANGUAGE") << std::endl;

  return 1;
}

int handlePull(const std::vector<std::string> &options, Localita &loc) {
  if (options.size() != 2) {
    std::cerr << loc.getText("PULL_USAGE") << std::endl;
    return 1;
  }
  if (!GitUtils::isGitInstalled()) {
    std::cerr << loc.getText("GIT_IS_NOT_INSTALLED") << std::endl;
    return 1;
  }

  auto center_templates_repo = Links::main_templates_repo();

  if (StringUtils::toLower(options.front()) == "template") {
    if (StringUtils::toLower(options.back()) == "center") {
      if (!GitUtils::runGitCloneOrPull(center_templates_repo,
                                       FileUtils::get_templates_folder().append(
                                           "community-templates"))) {
        std::cerr << loc.getText("SOMETHING_WENT_WRONG_WHILE_DOWNLOADING")
                  << std::endl;
        return 1;
      }
      std::cout << loc.getText("SUCCESSFULLY_INSTALLED_COMMUNITY_TEMPLATES")
                << std::endl;
      return 0;
    } else {
      std::string repoUrl = options.back();
      std::string repoName = GitUtils::getRepoNameFromUrl(repoUrl);

      if (repoName.empty()) {
        std::cerr << loc.getText("INVALID_REPO_URL") << ": " << repoUrl
                  << std::endl;
        return 1;
      }

      std::cout << loc.getText("REPOSITORY_NAME_DETECTED") << ": " << repoName
                << std::endl;
      std::cout << loc.getText("DOWNLOADING_INTO_REMOTE") << repoName
                << loc.getText("DIRECTORY") << std::endl;

      auto targetPath =
          FileUtils::get_templates_folder().append("remote").append(repoName);

      if (!GitUtils::runGitCloneOrPull(repoUrl, targetPath)) {
        std::cerr << loc.getText("SOMETHING_WENT_WRONG_WHILE_PULLING")
                  << repoUrl << std::endl;
        return 1;
      }

      std::cout << loc.getText("SUCCESSFULLY_INSTALLED_TEMPLATE") << repoUrl
                << std::endl;
      return 0;
      //-------------------------------------------------------------
    }
  } else if (StringUtils::toLower(options.front()) == "license") {
  } else {
    std::cerr << loc.getText("PLEASE_ENTER_A_VALID_OPTION") << std::endl;
    std::cerr << loc.getText("PULL_USAGE") << std::endl;
    return 1;
  }
  return 1;
}

int handleExtract(const std::string &directory, Localita &loc) {
  std::filesystem::path sourcePath(directory);
  if (!std::filesystem::exists(sourcePath) ||
      !std::filesystem::is_directory(sourcePath)) {
    std::cerr << "[" << loc.getText("ERROR") << "] "
              << loc.getText("INVALID_SOURCE_DIRECTORY") << ": " << directory
              << std::endl;
    return 1;
  }

  std::cout << loc.getText("CREATING_TEMPLATE_FROM") << directory << std::endl;

  std::string name, version, author, description;

  std::cout << loc.getText("EXTRACT_PROMPT_NAME");
  std::getline(std::cin, name);
  if (StringUtils::trim(name).empty()) {
    std::cerr << "[" << loc.getText("ERROR") << "] "
              << loc.getText("TEMPLATE_NAME_CANNOT_BE_EMPTY") << std::endl;
    return 1;
  }

  std::cout << loc.getText("EXTRACT_PROMPT_VERSION");
  std::getline(std::cin, version);
  if (StringUtils::trim(version).empty()) {
    version = "1.0";
  }

  std::cout << loc.getText("EXTRACT_PROMPT_AUTHOR");
  std::getline(std::cin, author);

  std::cout << loc.getText("EXTRACT_PROMPT_DESCRIPTION");
  std::getline(std::cin, description);

  CommandUtils::CommandMode cm = CommandUtils::CommandMode::Cautious;
  bool sm = false;

  std::cout << loc.getText("EXTRACT_PROMPT_COMMAND_MODE") << std::endl;
  std::cout << loc.getText("OPT_CAUTIOUS") << std::endl;
  std::cout << loc.getText("OPT_EXECUTE_ALL") << std::endl;
  std::cout << loc.getText("SELECT_ONE") << "[1-2]: ";
  int choice;
  std::cin >> choice;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  if (choice == 2) {
    cm = CommandUtils::CommandMode::ExecuteAll;
  }

  std::cout << loc.getText("EXTRACT_PROMPT_SILENT_MODE");
  char silent_choice;
  std::cin >> silent_choice;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  if (silent_choice == 'e' || silent_choice == 'E') {
    sm = true;
  }

  YAML::Node root;
  std::cout << loc.getText("EXTRACT_PROCESSING") << std::endl;

  bool success = Extractor::ExtractTemplate(sourcePath, root, name, version,
                                            author, description, cm, sm);

  if (success) {
    std::cout << "[" << loc.getText("SUCCESS") << "] "
              << loc.getText("TEMPLATE_EXTRACTED_SUCCESSFULLY") << name
              << std::endl;
    return 0;
  } else {
    std::cerr << "[" << loc.getText("ERROR") << "] "
              << loc.getText("TEMPLATE_EXTRACTION_FAILED") << std::endl;
    return 1;
  }
}
// ---------------- Main ----------------

int main(int argc, char **argv) {
  Cliopatra cli;
  cli.addOption("h", "help", Cliopatra::Option::bool_o);
  cli.addOption("v", "version", Cliopatra::Option::bool_o);
  cli.addOption("c", "create", Cliopatra::Option::bool_o);
  cli.addOption("a", "add", Cliopatra::Option::string_o);
  cli.addOption("b", "build", Cliopatra::Option::string_o);
  cli.addOption("co", "config", Cliopatra::Option::string_o);
  cli.addOption("v", "validate", Cliopatra::Option::bool_o);
  cli.addOption("t", "template", Cliopatra::Option::string_o);
  cli.addOption("l", "language", Cliopatra::Option::string_o);
  cli.addOption("p", "pull", Cliopatra::Option::multi_string_o);
  cli.addOption("r", "remote", Cliopatra::Option::string_o);
  cli.addOption("e", "extract", Cliopatra::Option::string_o);

  Localita loc;
  loc.setLocalePath(
      FileUtils::getDataDir(StringUtils::toLower(Metadata::name())) /
      "locales");
  loc.init();
  loc.setLocale(LocaleUtils::getPreferredLocale(loc));
  loc.setFallbackLocale("en");

  if (!loc.load()) {
    std::cerr << "Failed to load locale" << std::endl;
  }

  auto templates_dir = FileUtils::get_templates_folder();
  std::vector<std::filesystem::path> template_directories = {
      FileUtils::get_templates_folder()};

  try {
    auto results = cli.parse(argc, argv);
    if (results.find("extract") != results.end()) {
      return handleExtract(std::get<std::string>(results.at("extract")), loc);
    }
    if (results.find("pull") != results.end()) {
      return handlePull(std::get<std::vector<std::string>>(results.at("pull")),
                        loc);
    }

    if (results.find("language") != results.end()) {
      return handleLanguage(std::get<std::string>(results.at("language")), loc);
    }

    if (results.find("help") != results.end()) {
      OutputUtils::print_help();
      return 0;
    }
    if (results.find("version") != results.end()) {
      OutputUtils::print_version();
      return 0;
    }
    //! update
    if (results.find("validate") != results.end()) {
      if (results.find("remote") != results.end()) {
        std::cerr << loc.getText("CAN_ONLY_VALIDATE_LOCAL_TEMPLATES")
                  << std::endl;
        return 1;
      }
      if (!ensureTemplateProvided(results, loc))
        return 1;
      return handleValidate(template_directories,
                            std::get<std::string>(results.at("template")), loc);
    }

    if (results.find("create") != results.end()) {
      if (results.find("remote") != results.end()) {
        auto link = std::get<std::string>(results.at("remote"));
        auto repo_name = GitUtils::getRepoNameFromUrl(link);
        try {
          handlePull({"template", link}, loc);
        } catch (const std::exception &e) {
          std::cerr << e.what() << std::endl;
          std::cerr << loc.getText(
                           "SOMETHING_WENT_WRONG_WHILE_PULLING_TEMPLATE")
                    << std::endl;
          return 1;
        }
        auto template_dir = std::vector<std::filesystem::path>{
            FileUtils::get_templates_folder().append("remote").append(
                repo_name)};
        return handleCreate(template_dir, repo_name, loc);
      }
      if (!ensureTemplateProvided(results, loc))
        return 1;
      return handleCreate(template_directories,
                          std::get<std::string>(results.at("template")), loc);
    }

    std::cerr << "[" << loc.getText("ERROR") << "] "
              << loc.getText("NO_COMMAND_PROVIDED_ERROR") << std::endl;
    return 1;

  } catch (const std::exception &e) {
    std::cerr << "[" << loc.getText("EXCEPTION") << "] " << e.what()
              << std::endl;
    return 1;
  } catch (...) {
    std::cerr << "[" << loc.getText("ERROR") << "] "
              << loc.getText("UNEXPECTED_FAILURE_ERROR") << std::endl;
    return 1;
  }
}
