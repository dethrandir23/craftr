//content.hpp
#pragma once

#include <string>

class Content
{
private:
    std::string text;
public:
    Content() = default;
    Content(const std::string& text)    { this->text = text;     }
    const std::string get_text() const  { return text;           }
    void set_text(std::string new_text) { this->text = new_text; }
};