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
#include "../include/handle_helpers.hpp"
#include "../libs/localita/include/Localita.hpp"
#include <exception>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

// ---------------- Main ----------------

int main(int argc, char **argv) {
  Cliopatra cli;
  cli.addOption("h", "help", Cliopatra::Option::bool_o);
  cli.addOption("v", "version", Cliopatra::Option::bool_o);
  cli.addOption("c", "create", Cliopatra::Option::bool_o);
  cli.addOption("a", "add", Cliopatra::Option::string_o);
  cli.addOption("b", "build", Cliopatra::Option::bool_o);
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
        HandleHelpers::handleFind(std::get<std::string>(results.at("name")), loc);
      }
    }

    if (results.find("list") != results.end()) {
      if (results.find("name") != results.end()) {
        return HandleHelpers::handleList(std::get<std::string>(results.at("name")), loc);
      } else {
        return HandleHelpers::handleList("all", loc);
      }
    }

    if (results.find("extract") != results.end()) {
      return HandleHelpers::handleExtract(std::get<std::string>(results.at("extract")), loc);
    }
    if (results.find("pull") != results.end()) {
      return HandleHelpers::handlePull(std::get<std::vector<std::string>>(results.at("pull")),
                        loc);
    }

    if (results.find("language") != results.end()) {
      return HandleHelpers::handleLanguage(std::get<std::string>(results.at("language")), loc);
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
      if (!HandleHelpers::ensureTemplateProvided(results, loc))
        return 1;
      return HandleHelpers::handleValidate(template_directories,
                            std::get<std::string>(results.at("template")), loc);
    }

    if (results.find("create") != results.end()) {
      if (results.find("remote") != results.end()) {
        auto link = std::get<std::string>(results.at("remote"));
        auto repo_name = GitUtils::getRepoNameFromUrl(link);
        try {
          HandleHelpers::handlePull({"template", link}, loc);
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
        return HandleHelpers::handleCreate(template_dir, repo_name, loc);
      }
      if (!HandleHelpers::ensureTemplateProvided(results, loc))
        return 1;
      return HandleHelpers::handleCreate(template_directories,
                          std::get<std::string>(results.at("template")), loc);
    }

    if (results.find("add") != results.end()) {
      std::cout << "Not implemented yet" << std::endl;
      return 1;
    }

    if (results.find("build") != results.end()) {
      return HandleHelpers::handleBuild(loc);
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
