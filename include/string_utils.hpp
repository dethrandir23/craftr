//string_utils.hpp
#pragma once

#include <string>
#include "licensetype.hpp"

namespace StringUtils
{
    extern std::string project_name_replacer;
    extern std::string author_name_replacer;
    extern std::string date_replacer;

    std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);

    std::string GetCMakeLists(const std::string& project_name);

    std::string GetReadme(const std::string& project_name);

    std::string GetMetadata(const std::string& project_name, const std::string& author_name);

    std::string GetMainCpp(const std::string& project_name, const std::string& author_name);

    std::string GetLicense(const LicenseType& license, const std::string& author_name);
} // namespace StringUtils