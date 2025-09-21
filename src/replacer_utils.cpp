// replacer_utils.cpp
#include "../include/replacer_utils.hpp"
#include "../include/replacer.hpp"
#include <vector>

namespace ReplacerUtils {

void FillReplacersFromConfig(std::vector<Replacer> &replacers,
                             const Config &config) {
  for (auto &r : replacers) {
    if (auto it = config.variables.find(r.GetType());
        it != config.variables.end()) {
      r.SetValue(it->second);
    }
  }
}

void FillReplacersFromArgs(std::vector<Replacer> &replacers);
} // namespace ReplacerUtils