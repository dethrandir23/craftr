// strings.cpp
#include "../include/string_utils.hpp"
#include "../include/date_utils.hpp"
#include "../include/file_utils.hpp"
#include "../include/paths.hpp"
#include <iostream>
#include <string>

namespace StringUtils {

ReplacePairs GetReplacePairs(const std::string &project_name,
                             const std::string &author_name) {
  ReplacePairs pairs;

  pairs[Placeholder::ProjectName] = project_name;
  pairs[Placeholder::AuthorName] = author_name;
  pairs[Placeholder::Date] = DateUtils::GetCurrentYearStr();

  return pairs;
}

Contents GetContents(const LicenseType &license) {
  Contents contents;

  try {
    contents[ContentType::CMake] = FileUtils::LoadFileToString(Paths::CMake);
    contents[ContentType::Readme] = FileUtils::LoadFileToString(Paths::Readme);
    contents[ContentType::Metadata] =
        FileUtils::LoadFileToString(Paths::Metadata);
    contents[ContentType::Main] = FileUtils::LoadFileToString(Paths::Main);

    switch (license) {
    case LicenseType::MIT: {
      contents[ContentType::License] = FileUtils::LoadFileToString(Paths::MIT);
      break;
    }
    case LicenseType::BSD: {
      contents[ContentType::License] = FileUtils::LoadFileToString(Paths::BSD);
      break;
    }
    case LicenseType::GPL: {
      contents[ContentType::License] = FileUtils::LoadFileToString(Paths::GPL);
      break;
    }
    case LicenseType::Apache: {
      contents[ContentType::License] =
          FileUtils::LoadFileToString(Paths::Apache);
      break;
    }
    case LicenseType::Custom: {
      break; // TODO:
    }
    }
  } catch (const std::exception &e) {
    std::cerr << "Error loading templates: " << e.what() << std::endl;
  }

  return contents;
}

} // namespace StringUtils