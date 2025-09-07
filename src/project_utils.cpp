//project_utils.cpp

#include <filesystem>
#include <iostream>
#include <string>
#include "../include/Cliopatra.hpp"
#include "../include/output_utils.hpp"
#include "../include/project_utils.hpp"
#include "../include/file_utils.hpp"
#include "../include/string_utils.hpp"
#include "../include/licensetype.hpp"

namespace {
    bool create_cmake(const std::string& path, const std::string& name) {
        try {
            FileUtils::create_file(path, "CMakeLists.txt", StringUtils::GetCMakeLists(name));
        } catch (...) {
            return false;
        }
        return true;
    }

    bool create_readme(const std::string& path, const std::string& name) {
        try {
            FileUtils::create_file(path, "README.md", StringUtils::GetReadme(name));
        } catch (...) {
            return false;
        }
        return true;
    }

    bool create_metadata(const std::string& path, const std::string& name, const std::string& author) {
        try {
            FileUtils::create_file(path, "metadata.json", StringUtils::GetMetadata(name, author));
        } catch (...) {
            return false;
        }
        return true;
    }

    bool create_main_cpp(const std::string& path, const std::string& name, const std::string& author) {
        try {
            FileUtils::create_file(path, "main.cpp", StringUtils::GetMainCpp(name, author));
        } catch (...) {
            return false;
        }
        return true;
    }

    bool create_license(const std::string& path, const LicenseType& license, const std::string& author) {
        try  {
            FileUtils::create_file(path, "LICENSE", StringUtils::GetLicense(license, author));
        } catch (...) {
            return false;
        }
        return true;
    }
}

            // example project template to create:
            /*
            .
            ├── CMakeLists.txt
            ├── include
            ├── LICENSE
            ├── metadata
            │   └── metadata.json
            ├── README.md
            └── src
                └── main.cpp
            */
namespace ProjectUtils
{
    bool create_project(const std::string& name, const std::string& author, const LicenseType& license) {
        try {
            auto project_dir = FileUtils::get_project_subfolder(name);
            std::filesystem::create_directory(project_dir);

            std::filesystem::create_directory(project_dir / "include");
            std::filesystem::create_directory(project_dir / "src");
            std::filesystem::create_directories(project_dir / "metadata");

            create_cmake(project_dir, name);
            create_readme(project_dir, name);
            create_metadata(project_dir / "metadata", name, author);
            create_main_cpp(project_dir / "src", name, author);
            create_license(project_dir, license, author);

        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Filesystem error: " << e.what() << '\n';
            return false;
        } catch (...) {
            std::cerr << "Unknown error during project initialization\n";
            return false;
        }

        return true;
    }

    /*bool add_module(const std::string& name) {
        // TODO : not now
        return false;
    }*/
}