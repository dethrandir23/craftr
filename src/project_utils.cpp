// project_utils.cpp

#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

#include "../include/template_engine.hpp" 
#include "../include/string_utils.hpp"
#include "../include/file_utils.hpp"
#include "../include/licensetype.hpp"
#include "../include/project_utils.hpp" 

namespace ProjectUtils
{
    bool create_project(const std::string& name, const std::string& author, const LicenseType& license) {
        try {
            auto project_dir = FileUtils::get_project_subfolder(name);
            std::filesystem::create_directory(project_dir);

            std::filesystem::create_directory(project_dir / "include");
            std::filesystem::create_directory(project_dir / "src");
            std::filesystem::create_directories(project_dir / "metadata");

            StringUtils::ReplacePairs replacePairs = StringUtils::GetReplacePairs(name, author);
            StringUtils::Contents contents = StringUtils::GetContents(license);

            TemplateEngine::fillAllContents(contents, replacePairs);

            const std::unordered_map<StringUtils::ContentType, std::filesystem::path> file_paths = {
                { StringUtils::ContentType::CMake,    project_dir / "CMakeLists.txt" },
                { StringUtils::ContentType::Readme,   project_dir / "README.md" },
                { StringUtils::ContentType::License,  project_dir / "LICENSE" },
                { StringUtils::ContentType::Metadata, project_dir / "metadata" / "metadata.json" },
                { StringUtils::ContentType::Main,     project_dir / "src" / "main.cpp" }
            };

            if (license == LicenseType::Custom && contents.find(StringUtils::ContentType::License) == contents.end()) {
                std::cout << "Custom license selected, no LICENSE file will be created." << std::endl;
            }

            for (const auto& [contentType, contentString] : contents) {
                auto it = file_paths.find(contentType);
                if (it != file_paths.end()) {
                    const auto& file_path = it->second;
                    FileUtils::create_file(file_path.parent_path().string(), file_path.filename().string(), contentString);
                }
            }

        } catch (const std::exception& e) {
            std::cerr << "Error creating project: " << e.what() << '\n';
            return false;
        }

        std::cout << "Project '" << name << "' created successfully!" << std::endl;
        return true;
    }
}