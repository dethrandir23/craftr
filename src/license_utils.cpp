// license_utils.cpp
#include "../include/file_utils.hpp"
#include "../include/licensetype.hpp"
#include <algorithm>
#include <iostream>
#include <string>

namespace LicenseUtils {
LicenseType GetLicenseType(const std::string &name) {
  std::string licenseType = name;
  std::transform(licenseType.begin(), licenseType.end(), licenseType.begin(),
                 ::tolower);

  if (licenseType == "mit") {
    return LicenseType::MIT;
  } else if (licenseType == "bsd") {
    return LicenseType::BSD;
  } else if (licenseType == "gpl") {
    return LicenseType::GPL;
  } else if (licenseType == "apache") {
    return LicenseType::Apache;
  } else {
    return LicenseType::Custom;
  }
}

std::string GetLicenseText(const LicenseType &licenseType) {
  std::string licensePath;

  switch (licenseType) {
  case LicenseType::MIT:
    licensePath = "licenses/MIT.txt";
    break;
  case LicenseType::BSD:
    licensePath = "licenses/BSD.txt";
    break;
  case LicenseType::GPL:
    licensePath = "licenses/GPL.txt";
    break;  
  case LicenseType::Apache:
    licensePath = "licenses/APACHE.txt";
    break;
  case LicenseType::Custom:
    return "";
  default:
    return "";
  }

  std::filesystem::path fullPath =
      FileUtils::get_templates_folder() / licensePath;

  std::cout << "Loading license from: " << fullPath << std::endl;
  std::cout << "File exists: " << std::filesystem::exists(fullPath)
            << std::endl;

  return FileUtils::LoadFileToString(fullPath.string());
}
} // namespace LicenseUtils
