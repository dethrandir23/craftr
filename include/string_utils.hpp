//string_utils.hpp
#pragma once

#include <string>
#include <unordered_map>
#include "licensetype.hpp"

namespace StringUtils
{

    enum class Placeholder {
        ProjectName,
        AuthorName,
        Date
    };

    inline std::string ToString(Placeholder ph) {
        switch (ph) {
            case Placeholder::ProjectName: return "{{PROJECT_NAME}}";
            case Placeholder::AuthorName:  return "{{AUTHOR_NAME}}";
            case Placeholder::Date:        return "{{DATE}}";
        }
        return "";
    }

    enum class ContentType {
        CMake,
        Readme,
        Metadata,
        Main,
        License
    };

    inline std::string ToString(ContentType ct) {
        switch (ct) {
            case ContentType::CMake:    return "cmake";
            case ContentType::Readme:   return "readme";
            case ContentType::Metadata: return "metadata";
            case ContentType::Main:     return "main";
            case ContentType::License:  return "license";
        }
        return "";
    }

    using Contents = std::unordered_map<ContentType, std::string>;
    using ReplacePairs = std::unordered_map<Placeholder, std::string>;

    ReplacePairs GetReplacePairs(const std::string& project_name, const std::string& author_name);
    Contents GetContents(const LicenseType& license);
} // namespace StringUtils