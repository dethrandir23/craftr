//replacer.hpp
#pragma once
#include <string>

struct Replacer {
    std::string type;
    std::string text;
    std::string value;

    Replacer(const std::string& t1, const std::string& t2)
        : type(t1), text(t2) {}

    std::string GetType()  const { return type;  }
    std::string GetText()  const { return text;  }
    std::string GetValue() const { return value; }

    void SetType (const std::string& t) { type = t;  }
    void SetText (const std::string& t) { text = t;  }
    void SetValue(const std::string& v) { value = v; }

};