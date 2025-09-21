// project.hpp
#include "../include/project.hpp"
#include "../include/content_file.hpp"
#include "../include/file_utils.hpp"
#include "../include/license_utils.hpp"
#include "../include/template_engine.hpp"
#include <iostream>
#include <utility>
#include <vector>

void Project::createCommands() {
  std::vector<std::pair<std::string, std::string>> new_commands;
  for (auto &command : this->commands) {
    new_commands.emplace_back(std::make_pair(
        TemplateEngine::fillContent(command.first, this->replacers),
        TemplateEngine::fillContent(command.second, this->replacers)));
  }
  setCommands(new_commands);
}

void Project::createContentFiles() {
  for (auto &fbp : this->tmpl.GetFileBluePrints()) {
    if (fbp.target_path == "LICENSE") {
      if (getLicenseType() == "none") {
        continue;
      };

      auto licenseText = LicenseUtils::GetLicenseText(this->getLicenseType());
      if (!licenseText) {
        std::cerr << "Provided license does not exists in licenses folder. "
                     "Available licenses:"
                  << std::endl;
        for (auto l : LicenseUtils::GetLicenseTypes()) {
          std::cout << l << std::endl;
        }
        std::cout << "If you want to add a custom license, paste your license "
                     "template in TODO: add directory here";
        continue;
      }

      std::string filledLicense =
          TemplateEngine::fillContent(licenseText.value(), this->replacers);
      contentFiles.emplace_back(
          ContentFile(Content(filledLicense), fbp.target_path));
      continue;
    }
    std::filesystem::path fullTemplatePath =
        FileUtils::get_templates_folder() / fbp.template_path;

    contentFiles.emplace_back(
        ContentFile(Content(TemplateEngine::fillContent(
                        FileUtils::LoadFileToString(fullTemplatePath.string()),
                        this->replacers)),
                    fbp.target_path));
  }
}   