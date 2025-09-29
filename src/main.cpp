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

int handleList(const std::string &dir, Localita &loc) {
  std::filesystem::path path = FileUtils::get_templates_folder();

  auto selected_path = StringUtils::toLower(StringUtils::trim(dir));
  std::string display_name = selected_path;

  if (selected_path != "all") {
    if (selected_path == "user") {
      path /= "user";
      display_name = "User Templates";
    } else if (selected_path == "center") {
      path /= "community-templates";
      display_name = "Community Templates";
    } else if (selected_path == "remote") {
      path /= "remote";
      display_name = "Remote Templates";
    } else if (selected_path == "system") {
      path /= "system";
      display_name = "System Templates";
    } else {
      path /= dir;
      display_name = dir + " Templates";
    }
  } else {
    display_name = "All Templates";
  }

  auto templates = TemplateUtils::ListTemplates(path);

  constexpr int total_width = 50;

  {
    auto header = loc.getText("LIST_HEADER_TEMPLATES");
    int dash_count = total_width - 3 - static_cast<int>(header.size());
    if (dash_count < 0)
      dash_count = 0;

    std::string dashes;
    dashes.reserve(dash_count * 3);
    for (int i = 0; i < dash_count; i++)
      dashes += u8"─";

    std::cout << Colors::BOLD << Colors::CYAN << "┌─ " << header << " "
              << dashes << Colors::RESET << std::endl;
  }

  {
    std::cout << Colors::BOLD << Colors::CYAN << "│ " << Colors::RESET
              << Colors::YELLOW << display_name << Colors::RESET << std::endl;
  }

  {
    auto count_text = std::to_string(templates.size()) + " " +
                      loc.getText("LIST_TEMPLATE_COUNT");
    std::cout << Colors::BOLD << Colors::CYAN << "│ " << Colors::RESET
              << count_text << std::endl;
  }

  {
    std::string bottom;
    bottom.reserve((total_width - 1) * 3);
    for (int i = 0; i < total_width - 1; i++)
      bottom += u8"─";

    std::cout << Colors::BOLD << Colors::CYAN << "└" << bottom << Colors::RESET
              << std::endl;
  }

  if (templates.empty()) {
    std::cout << "  " << Colors::YELLOW << loc.getText("LIST_NO_TEMPLATES")
              << " " << display_name << Colors::RESET << std::endl;
    return 0;
  }

  for (const auto &t : templates) {
    std::cout << std::endl;

    auto metadata = TemplateUtils::GetTemplateMetadata(t.first);

    if (metadata) {
      std::cout << Colors::BOLD << Colors::GREEN << "  " << metadata->name;
      if (!metadata->version.empty()) {
        std::cout << Colors::MAGENTA << " v" << metadata->version;
      }
      std::cout << Colors::RESET << std::endl;

      if (!metadata->description.empty()) {
        std::cout << Colors::WHITE << "    " << metadata->description
                  << Colors::RESET << std::endl;
      }

      std::cout << Colors::CYAN << "    " << loc.getText("LIST_TEMPLATE_AUTHOR")
                << ": " << Colors::RESET;
      if (!metadata->author.empty()) {
        std::cout << metadata->author;
      } else {
        std::cout << Colors::YELLOW << loc.getText("LIST_UNKNOWN_AUTHOR")
                  << Colors::RESET;
      }
      std::cout << std::endl;

    } else {
      std::cout << Colors::BOLD << Colors::GREEN << "  " << t.second
                << Colors::RESET << std::endl;
    }

    std::cout << Colors::BLUE << "    " << loc.getText("LIST_TEMPLATE_PATH")
              << ": " << Colors::RESET << t.first.string() << std::endl;
  }

  std::cout << std::endl;
  return 0;
}

int handleCreateWithPath(const std::filesystem::path &templatePath, Localita &loc) {
    Config config;
    config.templatePath = templatePath;

    auto metadata = TemplateUtils::GetTemplateMetadata(templatePath);
    if (metadata) {
        std::cout << std::endl;
        std::cout << Colors::BOLD << Colors::GREEN << "Selected: " << metadata->name;
        if (!metadata->version.empty()) {
            std::cout << Colors::MAGENTA << " v" << metadata->version;
        }
        std::cout << Colors::RESET << std::endl;
        
        if (!metadata->description.empty()) {
            std::cout << Colors::WHITE << metadata->description << Colors::RESET << std::endl;
        }
        std::cout << std::endl;
    }

    for (auto key : TemplateUtils::GetTemplateReplacerTypes(config.templatePath)) {
        if (key == "DATE") {
            config.variables[key] = DateUtils::GetCurrentYearStr();
            continue;
        }
        std::string input;
        std::cout << Colors::CYAN << loc.getText("ENTER") << key << ": " << Colors::RESET;
        std::getline(std::cin, input);
        config.variables[key] = input;
        if (key == "PROJECT_NAME") {
            config.name = StringUtils::trim(input);
        }
    }

    std::cout << std::endl;
    std::cout << Colors::BOLD << Colors::CYAN << loc.getText("LICENSE_TYPE_SELECTION") << Colors::RESET << std::endl;
    for (auto l : LicenseUtils::GetLicenseTypes()) {
        std::cout << "- " << l << "\n";
    }
    std::cout << Colors::CYAN << loc.getText("SELECTED_LICENSE") << Colors::RESET;
    std::string input;
    std::getline(std::cin, input);
    config.license = StringUtils::trim(input).empty() ? "none" : StringUtils::trim(input);

    std::cout << std::endl;
    std::cout << Colors::BOLD << Colors::GREEN << "Creating project..." << Colors::RESET << std::endl;
    
    return ProjectUtils::create_project(config, loc) ? 0 : 1;
}

int handleFind(const std::string &name, Localita &loc) {
    std::filesystem::path base_path = FileUtils::get_templates_folder();
    std::vector<std::filesystem::path> templates_directories = {
        base_path / "user",
        base_path / "community-templates", 
        base_path / "remote",
        base_path / "system"
    };

    auto templates = TemplateUtils::FindTemplatesByName(templates_directories, name);

    // Header
    std::cout << Colors::BOLD << Colors::CYAN 
              << "┌─ " << loc.getText("FIND_HEADER") 
              << " ────────────────────────" << Colors::RESET << std::endl;
    
    std::cout << Colors::BOLD << Colors::CYAN << "│ " << Colors::RESET
              << templates.size() << " " << loc.getText("FOUND_TEMPLATES_FOR") 
              << " \"" << name << "\"" << std::endl;
    
    std::cout << Colors::BOLD << Colors::CYAN << "└──────────────────────────────────────────" 
              << Colors::RESET << std::endl;

    if (templates.empty()) {
        std::cout << "  " << Colors::YELLOW << loc.getText("FIND_NO_RESULTS") 
                  << " \"" << name << "\"" << Colors::RESET << std::endl;
        return 1;
    }

    for (size_t i = 0; i < templates.size(); ++i) {
        std::cout << std::endl;
        
        std::cout << Colors::BOLD << Colors::YELLOW << "  " << (i + 1) << ". " << Colors::RESET;
        
        auto metadata = TemplateUtils::GetTemplateMetadata(templates[i]);
        if (metadata) {
            std::cout << Colors::BOLD << Colors::GREEN << metadata->name;
            if (!metadata->version.empty()) {
                std::cout << Colors::MAGENTA << " v" << metadata->version;
            }
            std::cout << Colors::RESET << std::endl;
            
            if (!metadata->description.empty()) {
                std::cout << "     " << Colors::WHITE << metadata->description 
                          << Colors::RESET << std::endl;
            }
            
            std::cout << "     " << Colors::CYAN << loc.getText("LIST_TEMPLATE_AUTHOR") 
                      << ": " << Colors::RESET;
            if (!metadata->author.empty()) {
                std::cout << metadata->author;
            } else {
                std::cout << Colors::YELLOW << loc.getText("LIST_UNKNOWN_AUTHOR") 
                          << Colors::RESET;
            }
            
            std::string category = "Unknown";
            std::string path_str = templates[i].string();
            if (path_str.find("user") != std::string::npos) category = "User";
            else if (path_str.find("community-templates") != std::string::npos) category = "Community";
            else if (path_str.find("remote") != std::string::npos) category = "Remote";
            else if (path_str.find("system") != std::string::npos) category = "System";
            
            std::cout << "   " << Colors::BLUE << "Category: " << Colors::RESET 
                      << category << std::endl;
            
        } else {
            std::cout << Colors::BOLD << Colors::GREEN << templates[i].stem().string() 
                      << Colors::RESET << std::endl;
        }
        
        std::cout << "     " << Colors::BLUE << loc.getText("LIST_TEMPLATE_PATH") << ": " 
                  << Colors::RESET << templates[i].string() << std::endl;
    }

    std::cout << std::endl;
    std::cout << Colors::BOLD << Colors::CYAN 
              << loc.getText("FIND_SELECT_PROMPT").replace(loc.getText("FIND_SELECT_PROMPT").find("{count}"), 7, std::to_string(templates.size()))
              << Colors::RESET;
    
    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 0) {
        std::cout << Colors::YELLOW << loc.getText("FIND_CANCELLED") << Colors::RESET << std::endl;
        return 0;
    }

    if (choice < 1 || choice > static_cast<int>(templates.size())) {
        std::cerr << Colors::RED << loc.getText("FIND_INVALID_SELECTION") << Colors::RESET << std::endl;
        return 1;
    }

    std::filesystem::path selected_template = templates[choice - 1];
    
    return handleCreateWithPath(selected_template, loc);
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
  cli.addOption("va", "validate", Cliopatra::Option::bool_o);
  cli.addOption("t", "template", Cliopatra::Option::string_o);
  cli.addOption("l", "language", Cliopatra::Option::string_o);
  cli.addOption("p", "pull", Cliopatra::Option::multi_string_o);
  cli.addOption("r", "remote", Cliopatra::Option::string_o);
  cli.addOption("e", "extract", Cliopatra::Option::string_o);
  cli.addOption("li", "list", Cliopatra::Option::bool_o);
  cli.addOption("n", "name", Cliopatra::Option::string_o);
  cli.addOption("f", "find", Cliopatra::Option::bool_o);

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

    if (results.find("find") != results.end()) {
      if (results.find("name") == results.end()) {
        std::cerr << loc.getText("NAME_NOT_ENTERED") << std::endl;
      } else {
        handleFind(std::get<std::string>(results.at("name")), loc);
      }
    }

    if (results.find("list") != results.end()) {
      if (results.find("name") != results.end()) {
        return handleList(std::get<std::string>(results.at("name")), loc);
      } else {
        return handleList("all", loc);
      }
    }

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
      OutputUtils::print_help(loc);
      return 0;
    }
    if (results.find("version") != results.end()) {
      OutputUtils::print_version(loc);
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
