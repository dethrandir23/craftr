// project_utils.cpp

#include <exception>
#include <filesystem>
#include <iostream>
#include <string>

#include "../include/file_utils.hpp"
#include "../include/project.hpp"
#include "../include/project_template.hpp"
#include "../include/replacer_utils.hpp"
#include "../include/config.hpp"

namespace ProjectUtils {

bool create_project(const Config& config) {
  try {
    ProjectTemplate tmpl(config.templatePath);
    if (!tmpl.LoadTemplate()) {
      std::cerr << "Failed to load template\n";
      return false;
    }
    Project project(tmpl); //! Project uses old system, fix it later
    project.setName(config.name);
    project.setProjectSubFolder(FileUtils::get_project_subfolder(config.name));
    project.setLicenseType(config.license);
    ReplacerUtils::FillReplacersFromConfig(project.getReplacers(), config);
    project.createContentFiles();
    if (!FileUtils::write_project(project)) {
      std::cerr << "Something went wrong while writing project." << std::endl;
      return false;
    }

  } catch (const std::exception &e) {
    std::cerr << "Error creating project: " << e.what() << std::endl;
    return false;
  }
  std::cout << "Project '" << config.name << "' created successfully!" << std::endl;
  return true;
}
} // namespace ProjectUtils