//replacer.hpp
#pragma once

#include <string>
#include <vector>
#include "../include/string_utils.hpp"
#include "replacer.hpp"
#include "content.hpp"

class TemplateEngine
{
public:
    TemplateEngine() = delete;
    static std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);

    static void fillAll(std::string& content, const StringUtils::ReplacePairs& replaceFairs);
    static void fillAllContents(StringUtils::Contents& contents, const StringUtils::ReplacePairs& replacePairs);

    static std::string fillContent(std::string content, const std::vector<Replacer>& replacers);
    static void fillAll(Content& content, const std::vector<Replacer>& replacers);
    static void fillAllContents(std::vector<Content>& contents, const std::vector<Replacer>& replacers);
private:

};