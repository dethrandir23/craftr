// replacer_utils.cpp
#include "../include/replacer_utils.hpp"
#include "../include/replacer.hpp"
#include "../include/template_engine.hpp"
#include <vector>

namespace ReplacerUtils {

std::string ProcessString(const std::string &text, const std::vector<Replacer> &replacers) {
    return TemplateEngine::fillContent(text, replacers);
}

void FillReplacersFromConfig(std::vector<Replacer> &replacers,
                             const Config &config) {
  for (auto &r : replacers) {
    if (auto it = config.variables.find(r.GetType());
        it != config.variables.end()) {
      r.SetValue(it->second);
    }
  }
}

void FillFromMap(std::vector<Replacer> &replacers, 
                 const std::map<std::string, std::string> &vars) {
    for (auto &r : replacers) {
        if (auto it = vars.find(r.GetType()); it != vars.end()) {
            r.SetValue(it->second);
        }
    }
}
} // namespace ReplacerUtils