// command_utils.hpp
#include "metadata.hpp"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#pragma once

namespace CommandUtils {

#ifdef _WIN32
inline std::string hide_cmd = " > nul 2>&1";
#else
inline std::string hide_cmd = " > /dev/null 2>&1";
#endif

inline constexpr const char *GREEN = "\033[32m";
inline constexpr const char *RED = "\033[31m";
inline constexpr const char *RESET = "\033[0m";

enum class CommandMode { Cautious, ExecuteAll };

inline bool run_command(const std::string &cmd, bool silent_mode) {
  std::string final_cmd = silent_mode ? cmd + hide_cmd : cmd;

  int ret = system(final_cmd.c_str());
  if (ret == 0) {
    std::cout << GREEN << "[OK] " << RESET << cmd << std::endl;
    return true;
  } else {
    std::cout << RED << "[FAIL] " << RESET << cmd << " (code: " << ret << ")"
              << std::endl;
    return false;
  }
}

inline bool run_commands(const std::vector<std::string> &commands,
                         CommandMode mode, bool silent_mode) {
  bool all_ok = true;

  for (const auto &cmd : commands) {
    bool ok = run_command(cmd, silent_mode);

    if (!ok) {
      std::cerr << "Command failed:" << cmd << std::endl;
      all_ok = false;
      if (mode == CommandMode::Cautious) {
        return false;
      }
    }
  }

  return all_ok;
}

inline bool run_command_with_description(const std::string &cmd,
                                         const std::string &description,
                                         bool silent_mode) {
  if (!description.empty()) {
    std::cout << GREEN << "[INFO]" << RESET << description << std::endl;
  }
  if (!run_command(cmd, silent_mode)) {
    return false;
  }
  return true;
}

inline bool run_commands_with_description(
    const std::vector<std::pair<std::string, std::string>> &commands, CommandMode mode,
    bool silent_mode) {
  bool all_ok = true;

  for (const auto &cmd : commands) {
    bool ok = run_command_with_description(cmd.first, cmd.second, silent_mode);

    if (!ok) {
      std::cerr << "Command failed:" << cmd.first << std::endl;
      all_ok = false;
      if (mode == CommandMode::Cautious) {
        return false;
      }
    }
  }

  return all_ok;
}
} // namespace CommandUtils