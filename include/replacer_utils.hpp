// replacer_utils.hpp
#pragma once
#include "config.hpp"
#include "replacer.hpp"
#include <vector>

namespace ReplacerUtils {

std::string ProcessString(const std::string &text, const std::vector<Replacer> &replacers);

void FillReplacersFromConfig(std::vector<Replacer> &replacers,
                             const Config &config);

void FillFromMap(std::vector<Replacer> &replacers, const std::map<std::string, std::string> &vars);
} // namespace ReplacerUtils