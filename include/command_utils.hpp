// command_utils.hpp
#include "../libs/localita/include/Localita.hpp"
#include "colors.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#pragma once

namespace CommandUtils {

#ifdef _WIN32
inline std::string hide_cmd = " > nul 2>&1";
#else
inline std::string hide_cmd = " > /dev/null 2>&1";
#endif

enum class CommandMode { Cautious, ExecuteAll };

inline bool run_command(const std::string &cmd, bool silent_mode,
                        Localita &loc) {
  std::string final_cmd = silent_mode ? cmd + hide_cmd : cmd;

  int ret = system(final_cmd.c_str());
  if (ret == 0) {
    std::cout << Colors::GREEN << "[" << loc.getText("OK") << "] "
              << Colors::RESET << cmd << std::endl;
    return true;
  } else {
    std::cout << Colors::RED << "[" << loc.getText("FAIL") << "] "
              << Colors::RESET << cmd << " (" << loc.getText("CODE") << ": "
              << ret << ")" << std::endl;
    return false;
  }
}

inline bool run_commands(const std::vector<std::string> &commands,
                         CommandMode mode, bool silent_mode, Localita &loc) {
  bool all_ok = true;

  for (const auto &cmd : commands) {
    bool ok = run_command(cmd, silent_mode, loc);

    if (!ok) {
      std::cerr << loc.getText("COMMAND_FAILED") << ": " << cmd << std::endl;
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
                                         bool silent_mode, Localita &loc) {
  if (!description.empty()) {
    std::cout << Colors::GREEN << "[" << loc.getText("INFO") << "]"
              << Colors::RESET << " " << description << std::endl;
  }
  if (!run_command(cmd, silent_mode, loc)) {
    return false;
  }
  return true;
}

inline bool run_commands_with_description(
    const std::vector<std::pair<std::string, std::string>> &commands,
    CommandMode mode, bool silent_mode, Localita &loc) {
  bool all_ok = true;

  for (const auto &cmd : commands) {
    bool ok =
        run_command_with_description(cmd.first, cmd.second, silent_mode, loc);

    if (!ok) {
      std::cerr << loc.getText("COMMAND_FAILED") << ": " << cmd.first
                << std::endl;
      all_ok = false;
      if (mode == CommandMode::Cautious) {
        return false;
      }
    }
  }

  return all_ok;
}

inline bool run_commands_independently(
    const std::vector<std::pair<std::string, std::string>> &commands,
    Localita &loc) {
    
    bool all_ok = true;

    for (const auto &cmd_pair : commands) {
        const std::string& cmd = cmd_pair.first;
        const std::string& description = cmd_pair.second;

        if (!description.empty()) {
            std::cout << Colors::MAGENTA << description << Colors::RESET << std::endl;
        }

        std::cout << Colors::CYAN << "┌" << std::string(50, '-') << Colors::RESET << std::endl;
        std::cout << std::endl;

        int ret = system(cmd.c_str());

        std::cout << std::endl;
        std::cout << Colors::CYAN << "└" << std::string(50, '-') << Colors::RESET << std::endl;

        if (ret != 0) {
            std::cerr << Colors::RED << "[" << loc.getText("FAIL") << "] " 
                      << Colors::RESET << cmd << " (Code: " << ret << ")" << std::endl;
            all_ok = false;
            return false;
        }
        
        std::cout << std::endl; 
    }

    return all_ok;
}

} // namespace CommandUtils
