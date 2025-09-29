// output_utils.cpp
#include "../include/output_utils.hpp"
#include "../include/colors.hpp"
#include "../include/date_utils.hpp"
#include "../include/metadata.hpp"
#include "../libs/localita/include/Localita.hpp"
#include <iostream>

namespace OutputUtils {
void print_help(Localita &loc) {
  // Header
  std::cout << Colors::BOLD << Colors::CYAN << "┌─ "
            << loc.getText("HELP_HEADER") << " ────────────────────────"
            << Colors::RESET << std::endl;

  std::cout << Colors::BOLD << Colors::CYAN << "│ " << Colors::RESET
            << loc.getText("HELP_DESCRIPTION") << std::endl;

  std::cout << Colors::BOLD << Colors::CYAN << "│ " << Colors::RESET
            << std::endl;

  // Usage
  std::cout << Colors::BOLD << Colors::CYAN << "│ " << Colors::RESET
            << Colors::BOLD << loc.getText("USAGE") << Colors::RESET
            << std::endl;
  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << "craftr [OPTIONS] [COMMAND]" << std::endl;

  std::cout << Colors::BOLD << Colors::CYAN << "│ " << Colors::RESET
            << std::endl;

  // Basic Commands
  std::cout << Colors::BOLD << Colors::CYAN << "│ " << Colors::RESET
            << Colors::BOLD << loc.getText("BASIC_COMMANDS") << Colors::RESET
            << std::endl;

  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << Colors::BOLD << Colors::GREEN << "-h, --help" << Colors::RESET
            << "    " << loc.getText("HELP_DESCRIPTION_HELP") << std::endl;

  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << Colors::BOLD << Colors::GREEN << "-v, --version" << Colors::RESET
            << " " << loc.getText("HELP_DESCRIPTION_VERSION") << std::endl;

  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << Colors::BOLD << Colors::GREEN << "-l, --language <locale>"
            << Colors::RESET << "  " << loc.getText("HELP_DESCRIPTION_LANGUAGE")
            << std::endl;

  // Project Creation
  std::cout << Colors::BOLD << Colors::CYAN << "│ " << Colors::RESET
            << std::endl;
  std::cout << Colors::BOLD << Colors::CYAN << "│ " << Colors::RESET
            << Colors::BOLD << loc.getText("PROJECT_COMMANDS") << Colors::RESET
            << std::endl;

  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << Colors::BOLD << Colors::GREEN << "-c, --create" << Colors::RESET
            << "     " << loc.getText("HELP_DESCRIPTION_CREATE") << std::endl;

  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << Colors::BOLD << Colors::GREEN << "-t, --template <name>"
            << Colors::RESET << " " << loc.getText("HELP_DESCRIPTION_TEMPLATE")
            << std::endl;

  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << Colors::BOLD << Colors::GREEN << "-f, --find" << Colors::RESET
            << "       " << loc.getText("HELP_DESCRIPTION_FIND") << std::endl;

  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << Colors::BOLD << Colors::GREEN << "-n, --name <name>"
            << Colors::RESET << "    " << loc.getText("HELP_DESCRIPTION_NAME")
            << std::endl;

  // Template Management
  std::cout << Colors::BOLD << Colors::CYAN << "│ " << Colors::RESET
            << std::endl;
  std::cout << Colors::BOLD << Colors::CYAN << "│ " << Colors::RESET
            << Colors::BOLD << loc.getText("TEMPLATE_MANAGEMENT")
            << Colors::RESET << std::endl;

  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << Colors::BOLD << Colors::GREEN << "-li, --list [category]"
            << Colors::RESET << "  " << loc.getText("HELP_DESCRIPTION_LIST")
            << std::endl;

  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << Colors::BOLD << Colors::GREEN << "-p, --pull <type> <source>"
            << Colors::RESET << " " << loc.getText("HELP_DESCRIPTION_PULL")
            << std::endl;

  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << Colors::BOLD << Colors::GREEN << "-e, --extract <directory>"
            << Colors::RESET << " " << loc.getText("HELP_DESCRIPTION_EXTRACT")
            << std::endl;

  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << Colors::BOLD << Colors::GREEN << "-va, --validate"
            << Colors::RESET << "    "
            << loc.getText("HELP_DESCRIPTION_VALIDATE") << std::endl;

  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << Colors::BOLD << Colors::GREEN << "-r, --remote <url>"
            << Colors::RESET << "   " << loc.getText("HELP_DESCRIPTION_REMOTE")
            << std::endl;

  // Advanced Commands
  std::cout << Colors::BOLD << Colors::CYAN << "│ " << Colors::RESET
            << std::endl;
  std::cout << Colors::BOLD << Colors::CYAN << "│ " << Colors::RESET
            << Colors::BOLD << loc.getText("ADVANCED_COMMANDS") << Colors::RESET
            << std::endl;

  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << Colors::BOLD << Colors::GREEN << "-a, --add <module>"
            << Colors::RESET << "     " << loc.getText("HELP_DESCRIPTION_ADD")
            << std::endl;

  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << Colors::BOLD << Colors::GREEN << "-b, --build <target>"
            << Colors::RESET << "   " << loc.getText("HELP_DESCRIPTION_BUILD")
            << std::endl;

  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << Colors::BOLD << Colors::GREEN << "-co, --config <key=value>"
            << Colors::RESET << " " << loc.getText("HELP_DESCRIPTION_CONFIG")
            << std::endl;

  // Examples
  std::cout << Colors::BOLD << Colors::CYAN << "│ " << Colors::RESET
            << std::endl;
  std::cout << Colors::BOLD << Colors::CYAN << "│ " << Colors::RESET
            << Colors::BOLD << loc.getText("EXAMPLES") << Colors::RESET
            << std::endl;

  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << Colors::YELLOW << "# " << loc.getText("EXAMPLE_CREATE")
            << Colors::RESET << std::endl;
  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << "craftr --create --template cpp" << std::endl;

  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << Colors::YELLOW << "# " << loc.getText("EXAMPLE_FIND")
            << Colors::RESET << std::endl;
  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << "craftr --find --name cpp" << std::endl;

  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << Colors::YELLOW << "# " << loc.getText("EXAMPLE_LIST")
            << Colors::RESET << std::endl;
  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << "craftr --list" << std::endl;

  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << Colors::YELLOW << "# " << loc.getText("EXAMPLE_PULL")
            << Colors::RESET << std::endl;
  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << "craftr --pull template center" << std::endl;

  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << Colors::YELLOW << "# " << loc.getText("EXAMPLE_LANGUAGE")
            << Colors::RESET << std::endl;
  std::cout << Colors::BOLD << Colors::CYAN << "│   " << Colors::RESET
            << "craftr --language tr" << std::endl;

  // Footer
  std::cout << Colors::BOLD << Colors::CYAN << "│ " << Colors::RESET
            << std::endl;
  std::cout << Colors::BOLD << Colors::CYAN << "│ " << Colors::RESET
            << loc.getText("HELP_FOOTER") << std::endl;

  std::cout << Colors::BOLD << Colors::CYAN
            << "└──────────────────────────────────────────" << Colors::RESET
            << std::endl;
}

void print_version(Localita &loc) {
  std::cout << Colors::BOLD << Colors::GREEN << Metadata::name() << " "
            << Metadata::version() << Colors::RESET << std::endl;
  std::cout << loc.getText("VERSION_DESCRIPTION") << std::endl;
  std::cout << Colors::CYAN << "Copyright (c) "
            << DateUtils::GetCurrentYearStr() << " " << Metadata::author()
            << Colors::RESET << std::endl;
}
} // namespace OutputUtils