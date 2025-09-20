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
#include "../include/validate_utils.hpp"
#include <iostream>
#include <string>
#include <vector>

// ---------------- Helper Functions ----------------

bool ensureTemplateProvided(const Cliopatra::ParsedMap& results) {
    if (results.find("template") == results.end()) {
        std::cerr << "[ERROR] You must specify a template name using --template" << std::endl;
        return false;
    }
    return true;
}

int handleValidate(const std::string& templates_dir,
                   const std::string& templateName) {
    auto templatePathOpt = TemplateUtils::FindTemplateByName(templates_dir, templateName);
    if (!templatePathOpt.has_value()) {
        std::cerr << "[ERROR] Template not found: " << templateName << std::endl;
        return 1;
    }

    std::vector<std::string> errors;
    ValidateUtils::ValidateTemplate(templatePathOpt.value(), errors);

    if (errors.empty()) {
        std::cout << "[OK] Template '" << templateName << "' is valid ✅" << std::endl;
    } else {
        std::cout << "[FAIL] Template validation errors:\n";
        for (const auto& e : errors) {
            std::cout << "  - " << e << std::endl;
        }
        return 1;
    }

    return 0;
}

int handleCreate(const std::string& templates_dir,
                 const std::string& templateName) {
    Config config;

    auto templatePathOpt = TemplateUtils::FindTemplateByName(templates_dir, templateName);
    if (!templatePathOpt.has_value()) {
        std::cerr << "[ERROR] Template not found: " << templateName << std::endl;
        return 1;
    }
    config.templatePath = templatePathOpt.value();

    // Fill variables interactively
    for (auto key : TemplateUtils::GetTemplateReplacerTypes(templates_dir, templateName)) {
        if (key == "DATE") {
            config.variables[key] = DateUtils::GetCurrentYearStr();
            continue;
        }
        std::string input;
        std::cout << "Enter the " << key << ": ";
        std::getline(std::cin, input);
        config.variables[key] = input;
        if (key == "PROJECT_NAME") {
            config.name = StringUtils::trim(input);
        }
    }

    // License
    std::cout << "Select your license type:\n";
    for (auto l : LicenseUtils::GetLicenseTypes()) {
        std::cout << "- " << l << "\n";
    }
    std::cout << "Selected license (press enter for none): ";
    std::string input;
    std::getline(std::cin, input);
    config.license = StringUtils::trim(input).empty() ? "none" : StringUtils::trim(input);

    ProjectUtils::create_project(config);
    return 0;
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

    auto templates_dir = "../templates/config/craftr";

    try {
        auto results = cli.parse(argc, argv);

        if (results.find("help") != results.end()) {
            OutputUtils::print_help();
            return 0;
        }
        if (results.find("version") != results.end()) {
            OutputUtils::print_version();
            return 0;
        }

        if (results.find("validate") != results.end()) {
            if (!ensureTemplateProvided(results)) return 1;
            return handleValidate(templates_dir, std::get<std::string>(results.at("template")));
        }

        if (results.find("create") != results.end()) {
            if (!ensureTemplateProvided(results)) return 1;
            return handleCreate(templates_dir, std::get<std::string>(results.at("template")));
        }

        std::cerr << "[ERROR] No command provided. Use --help for usage." << std::endl;
        return 1;

    } catch (const std::exception &e) {
        std::cerr << "[EXCEPTION] " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "[ERROR] Unexpected failure while parsing arguments." << std::endl;
        return 1;
    }
}
