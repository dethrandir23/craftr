// project_utils.cpp

#include <exception>
#include <iostream>
#include <string>

#include "../include/command_utils.hpp"
#include "../include/config.hpp"
#include "../include/file_utils.hpp"
#include "../include/project.hpp"
#include "../include/project_template.hpp"
#include "../include/replacer_utils.hpp"
#include "../libs/localita/include/Localita.hpp"

namespace ProjectUtils {

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