#include "metadata.hpp"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

#pragma once

namespace CommandUtils {

constexpr const char *GREEN = "\033[32m";
constexpr const char *RED = "\033[31m";
constexpr const char *RESET = "\033[0m";

enum class CommandMode { Cautious, ExecuteAll };

inline bool run_command(const std::string &cmd) {
  int ret = system(cmd.c_str());
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
                         CommandMode mode = CommandMode::Cautious) {
  bool all_ok = true;

  for (const auto &cmd : commands) {
    bool ok = run_command(cmd);

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
} // namespace CommandUtils