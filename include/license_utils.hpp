// license_utils.hpp
#pragma once
#include "licensetype.hpp"
#include <string>

namespace LicenseUtils {
LicenseType GetLicenseType(const std::string &name);

std::string GetLicenseText(const LicenseType &licenseType);
} // namespace LicenseUtils