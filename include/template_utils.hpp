// template_utils.hpp
#pragma once
#include "project_template.hpp"
#include <filesystem>
#include <optional>
#include <vector>
#include <yaml-cpp/node/node.h>

namespace TemplateUtils {
std::optional<std::filesystem::path>
FindTemplateByName(const std::filesystem::path &directory,
                   const std::string &name);
std::vector<std::filesystem::path>
FindTemplatesByName(const std::vector<std::filesystem::path> &directories,
                    const std::string &name);

// ProjectTemplate LoadProjectTemplate(const YAML::Node &yml); // TODO implement
ProjectTemplate LoadProjectTemplate(const std::filesystem::path &path);
ProjectTemplate LoadProjectTemplate(const std::filesystem::path &directory,
                                    const std::string &name);

// std::vector<std::string> GetTemplateReplacerTypes(const std::string &yml); //
// TODO implement
std::vector<std::string> GetTemplateReplacerTypes(const std::string &path);
std::vector<std::string>
GetTemplateReplacerTypes(const std::filesystem::path &directory,
                         const std::string &name);
} // namespace TemplateUtils