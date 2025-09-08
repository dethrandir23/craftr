//replacer.hpp
#pragma once

#include <string>
#include "../include/string_utils.hpp"

class TemplateEngine
{
public:
    TemplateEngine() = delete;
    static std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);
    static void fillAll(std::string& content, const StringUtils::ReplacePairs& replaceFairs);
    static void fillAllContents(StringUtils::Contents& contents, const StringUtils::ReplacePairs& replacePairs);
private:

};