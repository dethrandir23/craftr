// output_utils.cpp
#include "../include/output_utils.hpp"
#include "../include/metadata.hpp"
#include <iostream>

namespace OutputUtils
{
    void print_help()
    {

        std::cout << "Usage: " << Metadata::name() << " [options]\n";
        std::cout << "  --help     Show help\n";
        std::cout << "  --version  Show version\n";
    }

    void print_version()
    {
        std::cout << Metadata::name()
                  << " v" << Metadata::version()
                  << " by " << Metadata::author() << "\n";
    }
}
