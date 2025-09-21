// locale_utils.hpp
#include "../libs/localita/include/Localita.hpp"
#include <string>

namespace LocaleUtils {

std::vector<std::string> getAvailableLocales(Localita &loc);
std::string getSystemLocale();
bool setConfigLocale(const std::string &locale);
bool setConfigLocaleSafe(const std::string &locale, Localita &loc);
std::string getConfigLocale();
std::string getPreferredLocale(Localita &loc); // can be set by user, or system default

} // namespace LocaleUtils