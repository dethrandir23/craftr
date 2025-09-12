    //replacer.hpp
    #include <string>
    #include <../include/replacer.hpp>
    #include "../include/template_engine.hpp"
    #include "../include/content.hpp"

    std::string TemplateEngine::ReplaceAll(std::string str, const std::string& from, const std::string& to) {
        size_t pos = 0;
        while ((pos = str.find(from, pos)) != std::string::npos)
        {
            str.replace(pos, from.length(), to);
            pos += to.length();
        }
        return str;
    }

    std::string TemplateEngine::fillContent(std::string content, const std::vector<Replacer>& replacers) {
        for (const auto& replacer : replacers) {
            content = ReplaceAll(content, replacer.GetText(), replacer.GetValue());
        }
        return content;
    }

    void TemplateEngine::fillAll(Content& content, const std::vector<Replacer>& replacers) {
        for (const auto& replacer : replacers) {
            content.set_text(ReplaceAll(content.get_text(), replacer.GetText(), replacer.GetValue()));
        }
    }

    void TemplateEngine::fillAllContents(std::vector<Content>& contents, const std::vector<Replacer>& replacers) {
        for (auto& content : contents) {
            fillAll(content, replacers);
        }
    }