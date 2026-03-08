// project_utils.cpp

#include <exception>
#include <iostream>
#include <string>
#include <filesystem>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <ostream>
#include <istream>

#include "../include/command_utils.hpp"
#include "../include/config.hpp"
#include "../include/file_utils.hpp"
#include "../include/project.hpp"
#include "../include/project_template.hpp"
#include "../include/replacer_utils.hpp"
#include "../libs/localita/include/Localita.hpp"
#include "../include/replacer_utils.hpp"

namespace ProjectUtils {

  bool saveBuildCommands(Project &project) {
    try {
        std::filesystem::path craftrDir = project.getProjectSubFolder() / ".craftr";
        
        if (!std::filesystem::exists(craftrDir)) {
            std::filesystem::create_directories(craftrDir);
        }

        YAML::Node buildNode;
        auto buildCmds = project.getBuildCommands();

        for (const auto& bc : buildCmds) {
            YAML::Node item;
            item["command"] = ReplacerUtils::ProcessString(bc.first, project.getReplacers());
            item["description"] = ReplacerUtils::ProcessString(bc.second, project.getReplacers());
            buildNode.push_back(item);
        }


        std::filesystem::path fname = "build.yaml";
        auto fpath = craftrDir / fname;

        std::ofstream fout(fpath, std::ios::trunc);
        fout << YAML::Dump(buildNode);

        return true;
    } catch (const std::exception &e) {
        std::cerr << "Build commands saving failed: " << e.what() << std::endl;
        return false;
    }
}

  bool saveRunCommands(Project &project) {
    try {
        std::filesystem::path craftrDir = project.getProjectSubFolder() / ".craftr";
        
        if (!std::filesystem::exists(craftrDir)) {
            std::filesystem::create_directories(craftrDir);
        }

        YAML::Node runNode;
        auto runCmds = project.getRunCommands();

        for (const auto& rc : runCmds) {
            YAML::Node item;
            item["command"] = ReplacerUtils::ProcessString(rc.first, project.getReplacers());
            item["description"] = ReplacerUtils::ProcessString(rc.second, project.getReplacers());
            runNode.push_back(item);
        }


        std::filesystem::path fname = "run.yaml";
        auto fpath = craftrDir / fname;

        std::ofstream fout(fpath, std::ios::trunc);
        fout << YAML::Dump(runNode);

        return true;
    } catch (const std::exception &e) {
        std::cerr << "Run commands saving failed: " << e.what() << std::endl;
        return false;
    }
}

bool create_project(const Config &config, Localita &loc) {
  try {
    ProjectTemplate tmpl(config.templatePath);
    if (!tmpl.LoadTemplate()) {
      std::cerr << loc.getText("FAILED_TO_LOAD_TEMPLATE") << std::endl;
      return false;
    }
    Project project(tmpl); //! Project uses old system, fix it later
    project.setName(config.name);
    project.setProjectSubFolder(FileUtils::get_project_subfolder(config.name));
    project.setLicenseType(config.license);
    ReplacerUtils::FillReplacersFromConfig(project.getReplacers(), config);
    project.createContentFiles();
    if (!FileUtils::write_project(project)) {
      std::cerr << loc.getText("SOMETHING_WENT_WRONG") << std::endl;
      return false;
    }

    if (!saveBuildCommands(project)) {
        std::cerr << "Warning: Could not save build metadata." << std::endl;
    }

    if (!saveRunCommands(project)) {
        std::cerr << "Warning: Could not save run metadata." << std::endl;
    }

    project.createCommands();
    if (!CommandUtils::run_commands_with_description(
            project.getCommands(), project.getCommandMode(),
            project.getSilentMode(), loc)) {
      std::cerr << Colors::RED << "[" << loc.getText("ERROR") << "] "
                << Colors::RESET << loc.getText("FAILED_TO_EXECUTE_COMMANDS")
                << std::endl;
      return false;
    } else {
      std::cout << Colors::GREEN << "[" << loc.getText("SUCCESS") << "] "
                << Colors::RESET
                << loc.getText("EXECUTED_ALL_COMMANDS_WITH_SUCCESS")
                << std::endl;
    }

  } catch (const std::exception &e) {
    std::cerr << Colors::RED << "[" << loc.getText("ERROR") << "] "
              << Colors::RESET << loc.getText("ERROR_CREATING_PROJECT")
              << e.what() << std::endl;
    return false;
  }
  std::cout << Colors::GREEN << "[" << loc.getText("SUCCESS") << "] "
            << loc.getText("PROJECT") << "'" << config.name << "' "
            << loc.getText("CREATED_SUCCESSFULLY") << std::endl;
  return true;
}
} // namespace ProjectUtils