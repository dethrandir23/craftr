// project.hpp
#include "../include/project.hpp"
#include "../include/content_file.hpp"
#include "../include/file_utils.hpp"
#include "../include/license_utils.hpp"
#include "../include/template_engine.hpp"
#include <iostream>

void Project::pairReplacers() {
  for (auto &replacer : replacers) {
    const std::string &type = replacer.GetType();

    if (type == "PROJECT_NAME") {
      replacer.SetValue(this->name);
    } else if (type == "AUTHOR_NAME") {
      replacer.SetValue(this->author);
    } else if (type == "DATE") {
      replacer.SetValue(this->date);
    } else {
      std::cerr << "Unknown replacer type: " << replacer.GetType() << "\n";
    }
  }
}

void Project::createContentFiles() {
  for (auto &fbp : this->tmpl.GetFileBluePrints()) {
    if (fbp.target_path == "LICENSE") {
      LicenseType lt = LicenseUtils::GetLicenseType(getLicenseType());
      std::cout << "License type enum: " << static_cast<int>(lt) << std::endl;

      std::string licenseText = LicenseUtils::GetLicenseText(lt);
      std::cout << "License text length: " << licenseText.length() << std::endl;

      std::string filledLicense =
          TemplateEngine::fillContent(licenseText, this->replacers);
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