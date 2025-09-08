    //replacer.hpp
    #include <string>
    #include "../include/template_engine.hpp"

    std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
        size_t pos = 0;
        while ((pos = str.find(from, pos)) != std::string::npos)
        {
            str.replace(pos, from.length(), to);
            pos += to.length();
        }
        return str;
    }

    void TemplateEngine::fillAll(std::string& content, const StringUtils::ReplacePairs& replaceFairs) {
        for (const auto& pair : replaceFairs) {
            content = ReplaceAll(content, StringUtils::ToString(pair.first), pair.second);
        }
    }

    void TemplateEngine::fillAllContents(StringUtils::Contents& contents, const StringUtils::ReplacePairs& replacePairs) {
        for (auto& content : contents) {
            fillAll(content.second, replacePairs);
        }
    }