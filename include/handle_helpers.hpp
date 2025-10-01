// handle_helpers.hpp
#pragma once

#include "../include/Cliopatra.hpp"
#include "../include/config.hpp"
#include "../include/extractor.hpp"
#include "../include/file_utils.hpp"
#include "../include/git_utils.hpp"
#include "../include/license_utils.hpp"
#include "../include/links.hpp"
#include "../include/locale_utils.hpp"
#include "../include/metadata.hpp"
#include "../include/output_utils.hpp"
#include "../include/project_utils.hpp"
#include "../include/string_utils.hpp"
#include "../include/template_utils.hpp"
#include "../include/validate_utils.hpp"
#include "../libs/localita/include/Localita.hpp"
#include <exception>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

namespace HandleHelpers {
bool ensureTemplateProvided(const Cliopatra::ParsedMap &results,
                            Localita &loc);

int handleValidate(
    const std::vector<std::filesystem::path> &templates_directories,
    const std::string &templateName, Localita &loc);
int handleCreate(
    const std::vector<std::filesystem::path> &templates_directories,
    const std::string &templateName, Localita &loc);

int handleLanguage(const std::string &newLocale, Localita &loc);
int handlePull(const std::vector<std::string> &options, Localita &loc);

int handleExtract(const std::string &directory, Localita &loc);

int handleList(const std::string &dir, Localita &loc);

int handleCreateWithPath(const std::filesystem::path &templatePath,
                         Localita &loc);

int handleFind(const std::string &name, Localita &loc);
} // namespace HandleHelpers