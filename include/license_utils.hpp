// license_utils.hpp
#pragma once
#include <optional>
#include <string>
#include <vector>

namespace LicenseUtils {

std::optional<std::string> GetLicenseText(const std::string &name);
std::vector<std::string> GetLicenseTypes();
} // namespace LicenseUtils