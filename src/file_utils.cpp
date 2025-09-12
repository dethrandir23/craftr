//file_utils.cpp
#include "../include/file_utils.hpp"
#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>

namespace FileUtils
{
    bool create_file(const std::filesystem::path& path, const std::string& filename, const std::string& content) {
        try {
            if (!path.empty() && !std::filesystem::exists(path)) {
                std::filesystem::create_directories(path);
            }
            std::filesystem::path fullPath = path / filename;
            std::ofstream file(fullPath);
            if (!file.is_open()) {
                std::cerr << "Error: Could not open file for writing: " << fullPath << std::endl;
                return false;
            }   
            file << content;
            file.close();
        } catch (const std::exception& e) {
            std::cerr << "Error creating file: " << e.what() << std::endl;
            return false;
        }

        return true;
    }


    std::filesystem::path get_project_subfolder(const std::string& project_name) {
        std::filesystem::path currentDir = std::filesystem::current_path();
        return currentDir / project_name;
    }

    std::string LoadFileToString(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Can't open file: " + filepath);
        }

        std::ostringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    std::filesystem::path get_project_root() {
        return PROJECT_ROOT_DIR;
    }

    std::filesystem::path get_templates_folder() {
        return get_project_root().append("templates");
    }

    bool write_content_files(const std::vector<ContentFile>& files) {
        try {
            for (const auto& file_to_create : files) {
                const std::filesystem::path& target = file_to_create.target_path;

                const std::string& content_text = file_to_create.content.get_text();

                std::filesystem::path directory = target.parent_path();
                std::string filename = target.filename().string();
                
                if (!create_file(directory, filename, content_text)) {
                    std::cerr << "Failed to create one or more files. Aborting." << std::endl;
                    return false; 
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "An error occurred while writing files: " << e.what() << std::endl;
            return false;
        }

        return true;
    }

    bool create_subfolders(const std::vector<std::filesystem::path>& subfolders, const std::filesystem::path& basePath) {
        try {
            for (const auto& folder : subfolders) {
                std::filesystem::path fullPath = basePath / folder;
                if (!std::filesystem::exists(fullPath)) {
                    std::filesystem::create_directories(fullPath);
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error creating subfolders: " << e.what() << std::endl;
            return false;
        }
        return true;
    }

} // namespace FileUtils
