// project_utils.hpp
#pragma once

#include "config.hpp"
#include <string>
#include "../libs/localita/include/Localita.hpp"

namespace ProjectUtils {
bool create_project(const Config &config, Localita &loc);
/*bool add_module(const std::string& name);*/

} // namespace ProjectUtils