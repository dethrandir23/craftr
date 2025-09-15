// date_utils.cpp

#include <ctime>
#include <string>

namespace DateUtils {
std::string GetCurrentYearStr() {
  std::time_t t = std::time(nullptr);
  std::tm *const pTInfo = std::localtime(&t);
  return std::to_string(pTInfo->tm_year + 1900);
}
} // namespace DateUtils