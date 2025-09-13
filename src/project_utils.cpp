// project_utils.cpp

#include <filesystem>
#include <iostream>
#include <string>

#include "../include/file_utils.hpp"
#include "../include/project.hpp"
#include "../include/project_template.hpp"

namespace ProjectUtils
{

    bool create_project(const std::string& name, const std::string& author, const std::string& license) {
        try {

            std::filesystem::path templatePath = FileUtils::get_templates_folder() / "config/craftr/cpp.yaml";
            ProjectTemplate tmpl(templatePath);
            if (!tmpl.LoadTemplate()) {
                std::cerr << "Failed to load template\n";
                return false;
            }
            Project project(name, "1.0.0", author, tmpl);
            project.setProjectSubFolder(FileUtils::get_project_subfolder(project.getName()));
            project.setLicenseType(license);
            project.pairReplacers();
            project.createContentFiles();
            if (!FileUtils::write_project(project)) { return false; }

        } catch (const std::exception& e) {
            std::cerr << "Error creating project: " << e.what() << '\n';
            return false;
        } 
        std::cout << "Project '" << name << "' created successfully!" << std::endl;
        return true;
    }
}