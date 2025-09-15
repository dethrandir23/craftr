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
#include "../include/output_utils.hpp"
#include "../include/project_utils.hpp"
#include <iostream>
#include <ostream>

bool execResults(Cliopatra::ParsedMap &results) {
  try {

    if (results.find("help") != results.end()) {
      OutputUtils::print_help();
    }

    if (results.find("version") != results.end()) {
      OutputUtils::print_version();
    }

    if (results.find("create") != results.end()) {
      // TODO : Add optional licence, readme etc flags.
      auto name = std::get<std::string>(results["create"]);
      auto author = std::get<std::string>(results["author"]);
      auto license = std::get<std::string>(results["license"]);
      if (!name.empty()) {
        ProjectUtils::create_project(
            name, author, (std::get<std::string>(results["license"])));
      } else {
        std::cout << "Please enter a project name" << std::endl;
      }
    }

    if (results.find("") != results.end()) {
    }

    return true;
  } catch (...) {
    return false;
  }
}

int main(int argc, char **argv) {
  Cliopatra cliopatra;
  cliopatra.addOption("h", "help", Cliopatra::Option::bool_o);
  cliopatra.addOption("v", "version", Cliopatra::Option::bool_o);
  cliopatra.addOption("c", "create", Cliopatra::Option::string_o);
  cliopatra.addOption("a", "add", Cliopatra::Option::string_o);
  cliopatra.addOption("b", "build", Cliopatra::Option::string_o);
  cliopatra.addOption("au", "author", Cliopatra::Option::string_o);
  cliopatra.addOption("l", "license", Cliopatra::Option::string_o);

  try {
    auto results = cliopatra.parse(argc, argv);
    execResults(results);
  } catch (...) {
    std::cerr << "Parsing command line arguments or executing them."
              << std::endl;
    return 1;
  }

  return 0;
}