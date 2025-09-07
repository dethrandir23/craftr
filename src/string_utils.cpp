// strings.cpp
#include "../include/string_utils.hpp"
#include "../include/date_utils.hpp"
#include <string>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>

namespace StringUtils
{
    std::string project_name_replacer = "PROJECT_NAME";
    std::string author_name_replacer = "AUTHOR_NAME";
    std::string date_replacer = "DATE";

    std::string LoadFileToString(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Can't open file: " + filepath);
        }

        std::ostringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
        size_t pos = 0;
        while ((pos = str.find(from, pos)) != std::string::npos)
        {
            str.replace(pos, from.length(), to);
            pos += to.length();
        }
        return str;
    }

    std::string GetCMakeLists(const std::string& project_name) {
        auto cmake = LoadFileToString("../templates/cmake/CMake_Template.txt");
        auto result = ReplaceAll(cmake, project_name_replacer, project_name);
        return result;
    }

    std::string GetReadme(const std::string& project_name) {
        auto readme = LoadFileToString("../templates/readme/Readme.txt");
        auto result = ReplaceAll(readme, project_name_replacer, project_name);
        return result;
    }

    std::string GetMetadata(const std::string& project_name, const std::string& author_name) {
        auto metadata = LoadFileToString("../templates/metadata/metadata.txt");
        auto metadata2 = ReplaceAll(metadata, project_name_replacer, project_name);
        auto result = ReplaceAll(metadata2, author_name_replacer, author_name);
        return result;
    }

    std::string GetMainCpp(const std::string& project_name, const std::string& author_name) {
        auto maincpp = LoadFileToString("../templates/cpp/main.txt");
        auto maincpp2 = ReplaceAll(maincpp, project_name_replacer, project_name);
        auto maincpp3 = ReplaceAll(maincpp2, author_name_replacer, author_name);
        auto result = ReplaceAll(maincpp3, date_replacer, DateUtils::GetCurrentYearStr());
        return result;
    }

    std::string GetLicense(const LicenseType& license, const std::string& author_name) {
        switch (license) {
            case LicenseType::MIT: {
                auto license_content = LoadFileToString("../templates/licenses/MIT.txt");
                auto result = ReplaceAll(license_content, author_name_replacer, author_name);
                auto final_result = ReplaceAll(result, date_replacer, DateUtils::GetCurrentYearStr());
                return final_result;
            }
            case LicenseType::BSD: {
                auto license_content = LoadFileToString("../templates/licenses/BSD.txt");
                auto result = ReplaceAll(license_content, author_name_replacer, author_name);
                auto final_result = ReplaceAll(result, date_replacer, DateUtils::GetCurrentYearStr());
                return final_result;
            }
            case LicenseType::GPL: {
                auto license_content = LoadFileToString("../templates/licenses/GPL.txt");
                auto final_result = ReplaceAll(license_content, date_replacer, DateUtils::GetCurrentYearStr());
                return final_result;
            }
            case LicenseType::Apache: {
                auto license_content = LoadFileToString("../templates/licenses/APACHE.txt");
                auto result = ReplaceAll(license_content, author_name_replacer, author_name);
                auto final_result = ReplaceAll(result, date_replacer, DateUtils::GetCurrentYearStr());
                return final_result;
            }
            case LicenseType::Custom: {
                return "No license specified or custom license not yet implemented."; // TODO : 
            }
        }
        return "";
    }

} // namespace StringUtils

