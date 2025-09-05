//project_utils.cpp

#include <iostream>
#include "../include/Cliopatra.hpp"
#include "../include/output_utils.hpp"
#include "../include/project_utils.hpp"

namespace ProjectUtils
{
    bool init_project(const std::string& name) {
        try {
            // example project template to create:
            /*
            .
            ├── CMakeLists.txt
            ├── include
            ├── LICENSE
            ├── metadata
            │   └── metadata.json
            ├── README.md
            └── src
                └── main.cpp
            */



        } catch (...) {
            return false;
        }

        return true;
    }

    bool add_module(const std::string& name) {

    }
}