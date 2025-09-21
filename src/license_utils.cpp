// license_utils.cpp
#include "../include/file_utils.hpp"
#include "../include/string_utils.hpp"
#include <algorithm>
#include <optional>
#include <string>

namespace LicenseUtils {

std::vector<std::string> GetLicenseTypes() {
  return FileUtils::getFileNames(
      FileUtils::getDataDir(StringUtils::toLower(Metadata::name())).append("licenses"));
}

std::optional<std::string> GetLicenseText(const std::string &name) {
  std::string licenseType = name;
  std::transform(licenseType.begin(), licenseType.end(), licenseType.begin(),
                 ::tolower);

  std::vector<std::string> licenses = GetLicenseTypes();
  auto it = std::find(licenses.begin(), licenses.end(), licenseType);
  if (it != licenses.end()) {
    auto licensesMap = FileUtils::getFileMap(
        FileUtils::getDataDir(StringUtils::toLower(Metadata::name())).append("licenses"));
    return FileUtils::LoadFileToString(licensesMap[licenseType]);
  } else {
    return std::nullopt;
  }
}

} // namespace LicenseUtils
