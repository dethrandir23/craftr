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
#include "../include/config_utils.hpp"
#include "../include/file_utils.hpp"
#include "../include/license_utils.hpp"
#include "../include/output_utils.hpp"
#include "../include/project_utils.hpp"
#include "../include/string_utils.hpp"
#include "../include/template_utils.hpp"
#include <iostream>
#include <ostream>
#include <string>

int main(int argc, char **argv) {
  Cliopatra cliopatra;
  cliopatra.addOption("h", "help", Cliopatra::Option::bool_o);
  cliopatra.addOption("v", "version", Cliopatra::Option::bool_o);
  cliopatra.addOption("c", "create", Cliopatra::Option::bool_o);
  cliopatra.addOption("a", "add", Cliopatra::Option::string_o);
  cliopatra.addOption("b", "build", Cliopatra::Option::string_o);
  cliopatra.addOption("co", "config", Cliopatra::Option::string_o);
  cliopatra.addOption("t", "template", Cliopatra::Option::string_o);

  try {
    auto results = cliopatra.parse(argc, argv);
    Config config;
    auto templates_dir = "../templates/config/craftr";
    if (results.find("create") != results.end()) {
      if (results.find("template") == results.end()) {
        std::cerr << "You must enter a template name to create the project"
                  << std::endl;
      }
      config.templatePath =
          TemplateUtils::FindTemplateByName(
              templates_dir, std::get<std::string>(results["template"]))
              .value();
      for (auto valuetoask : TemplateUtils::GetTemplateReplacerTypes(
               templates_dir, std::get<std::string>(results["template"]))) {
        if (valuetoask == "DATE") {
          config.variables[valuetoask] = DateUtils::GetCurrentYearStr();
          continue;
        }
        std::string input;
        std::cout << "Enter the " << valuetoask << " : ";
        std::getline(std::cin, input);
        config.variables[valuetoask] = input;
        if (valuetoask == "PROJECT_NAME") {
          config.name = StringUtils::trim(input);
        }
      }
      std::string input;
      std::cout << "Select your license type: " << std::endl;
      for (auto l : LicenseUtils::GetLicenseTypes()) {
        std::cout << "- " << l << std::endl;
      }
      std::cout << "Selected license (if you don't want one, press enter): "
                << std::endl;
      std::getline(std::cin, input);
      config.license = StringUtils::trim(input) == "" ? config.license = "none"
                                                      : config.license = input;
    }
    ProjectUtils::create_project(config);
  } catch (...) {
    std::cerr << "Parsing command line arguments or executing them."
              << std::endl;
    return 1;
  }

  return 0;
}