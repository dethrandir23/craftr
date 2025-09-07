//project_utils.hpp
#pragma once

#include "licensetype.hpp"
#include <string>

namespace ProjectUtils
{
    bool create_project(const std::string& name, const std::string& author, const LicenseType& license);
    /*bool add_module(const std::string& name);*/
    
}