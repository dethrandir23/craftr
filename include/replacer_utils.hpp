// replacer_utils.hpp
#pragma once
#include "config.hpp"
#include "replacer.hpp"
#include <vector>

namespace ReplacerUtils {

void FillReplacersFromConfig(std::vector<Replacer> &replacers,
                             const Config &config);

void FillReplacersFromArgs(std::vector<Replacer> &replacers);
} // namespace ReplacerUtils