// output_utils.cpp
#include "../include/output_utils.hpp"
#include "../include/metadata.hpp"
#include <iostream>

namespace OutputUtils
{
    void print_help()
    {
        std::cout << "Usage: " << Metadata::name() << " [options]\n\n";
        std::cout << "Options:\n";
        std::cout << "  -h, --help             Show this help message\n";
        std::cout << "  -v, --version          Show tool version\n";
        std::cout << "  -c, --create <name>    Create a new project with the given name\n";
        std::cout << "  -a, --add <module>     Add a new module/component to the project\n";
        std::cout << "  -b, --build <target>   Build the project or specific target\n";
        std::cout << "  --au, --author <name>  Specify the author of the project\n";
        std::cout << "  -l, --license <type>   Set the license (MIT, BSD, GPL, Apache, Custom)\n\n";
        
        std::cout << "Examples:\n";
        std::cout << "  " << Metadata::name() << " --create MyApp --author \"John Doe\" --license MIT\n";
        std::cout << "  " << Metadata::name() << " --add utils\n";
        std::cout << "  " << Metadata::name() << " --build all\n";
    }

    void print_version()
    {
        std::cout << Metadata::name()
                  << " v" << Metadata::version()
                  << " by " << Metadata::author() << "\n";
    }
}
