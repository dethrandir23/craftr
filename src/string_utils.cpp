// strings.cpp
#include "../include/string_utils.hpp"
#include <algorithm>
#include <string>

namespace StringUtils {

std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return s;
}

} // namespace StringUtils