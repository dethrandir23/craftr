//project_template.hpp
#pragma once

#include "file_blueprint.hpp"
#include <filesystem>
#include <string>
#include <vector>

class ProjectTemplate {
public:
    ProjectTemplate(const std::filesystem::path& template_path) {
        this->template_path = template_path;
    }

    bool LoadTemplate();

private:
    std::string name = "";
    std::string version = "";
    std::string author = "";
    std::string description = "";

    std::vector<std::pair<std::string, std::string>> replacers;

    std::vector<std::filesystem::path> subfolders;
    std::vector<FileBlueprint> file_blueprints;

    std::filesystem::path template_path;
};