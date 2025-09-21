// locale_utils.cpp
#include "../include/locale_utils.hpp"
#include "../include/file_utils.hpp"
#include "../include/metadata.hpp"
#include "../include/string_utils.hpp"
#include "../libs/localita/include/Localita.hpp"
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <string>
#if defined(_WIN32)
#include <windows.h>
#endif

namespace {
inline std::string normalizeLocale(const std::string &raw) {
  if (raw.size() >= 2) {
    std::string lang = raw.substr(0, 2);
    for (auto &c : lang)
      c = std::tolower(c);
    return lang;
  }
  return "en";
}
} // namespace

namespace LocaleUtils {

std::vector<std::string> getAvailableLocales(Localita &loc) {
  return loc.listLocales();
}

std::string getSystemLocale() {
  const char *envVars[] = {"LC_ALL", "LC_MESSAGES", "LANG"};
  for (auto var : envVars) {
    const char *val = std::getenv(var);
    if (val && *val) {
      return normalizeLocale(val);
    }
  }

#if defined(_WIN32)
  WCHAR localeName[LOCALE_NAME_MAX_LENGTH];
  if (GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH)) {
    char buffer[LOCALE_NAME_MAX_LENGTH];
    WideCharToMultiByte(CP_UTF8, 0, localeName, -1, buffer,
                        LOCALE_NAME_MAX_LENGTH, NULL, NULL);
    return normalizeLocale(buffer);
  }
#endif

  return "en";
}

bool setConfigLocale(const std::string &locale) {
  auto config_dir =
      FileUtils::getConfigDir(StringUtils::toLower(Metadata::name()));
  auto locale_file_path = config_dir / "locale.config";
  try {
    std::ofstream LocaleFile(locale_file_path.string());
    LocaleFile << locale;
    LocaleFile.close();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
  return true;
}

bool setConfigLocaleSafe(const std::string &locale, Localita &loc) {
  auto locales = loc.listLocales();
  auto it = std::find(locales.begin(), locales.end(), locale);
  if (it == locales.end()) {
    return false;
  }

  return setConfigLocale(locale);
}

std::string getConfigLocale() {
  std::string preferredLocale = "";
  auto config_dir =
      FileUtils::getConfigDir(StringUtils::toLower(Metadata::name()));
  auto locale_file_path = config_dir / "locale.config";

  try {
    if (std::filesystem::exists(locale_file_path)) {
      std::ifstream localeFile(locale_file_path.string());
      std::getline(localeFile, preferredLocale);
      localeFile.close();

      if (preferredLocale.empty()) {
        if (setConfigLocale("system")) {
          preferredLocale = "system";
        }
      }
    } else {
      if (setConfigLocale("system")) {
        preferredLocale = "system";
      }
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }

  return preferredLocale;
}

// can be set by user, or system default
std::string getPreferredLocale(Localita &loc) {
  std::string preferredLocale = "";
  try {
    preferredLocale = getConfigLocale();

    if (preferredLocale.empty() || preferredLocale == "system") {
      preferredLocale = getSystemLocale();
    } else {
      auto locales = loc.listLocales();
      auto it = std::find(locales.begin(), locales.end(), preferredLocale);
      if (it == locales.end()) {
        preferredLocale = getSystemLocale();
      }
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    preferredLocale = "en";
  }
  return preferredLocale;
}
} // namespace LocaleUtils