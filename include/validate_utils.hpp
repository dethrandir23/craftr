// validate_utils.hpp
#pragma once

#include "../libs/localita/include/Localita.hpp"
#include <filesystem>
#include <vector>
#include <yaml-cpp/node/node.h>

namespace ValidateUtils {

bool ValidateTemplate(const YAML::Node &tmpl, std::vector<std::string> &errors,
                      Localita &loc);
bool ValidateTemplate(const std::filesystem::path &path,
                      std::vector<std::string> &errors, Localita &loc);
} // namespace ValidateUtils