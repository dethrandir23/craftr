// string_utils.hpp
#pragma once

#include <string>
#include <algorithm>
#include <cctype>
namespace StringUtils {

inline std::string toLower(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return s;
}

inline std::string trim_left(const std::string &s) {
    auto start = std::find_if(s.begin(), s.end(),
                              [](unsigned char ch) { return !std::isspace(ch); });
    return std::string(start, s.end());
}

inline std::string trim_right(const std::string &s) {
    auto end = std::find_if(s.rbegin(), s.rend(),
                            [](unsigned char ch) { return !std::isspace(ch); }).base();
    return std::string(s.begin(), end);
}

inline std::string trim(const std::string &s) {
    return trim_left(trim_right(s));
}

} // namespace StringUtils