// extractor.hpp
#pragma once
#include "command_utils.hpp"
#include "file_blueprint.hpp"
#include <filesystem>
#include <yaml-cpp/node/node.h>

namespace Extractor {

bool ExtractFiles(const std::filesystem::path &directory,
                  std::vector<FileBlueprint> &fbp);

bool ExtractSubfolders(const std::filesystem::path &path,
                       std::vector<std::filesystem::path> &sf);

bool CopyFileTemplates(const std::filesystem::path &dst,
                       const std::vector<FileBlueprint> &fbp);

bool AddFiles(YAML::Node &root, const std::vector<FileBlueprint> &fbp);

bool AddSubfolders(YAML::Node &root,
                   const std::vector<std::filesystem::path> &sf);

bool AddMetadata(YAML::Node &root, const std::string &name,
                 const std::string &version, const std::string &author,
                 const std::string &description);

bool AddDefaultVariables(YAML::Node &root);

bool AddCommandMode(YAML::Node &root, const CommandUtils::CommandMode &cm);

bool AddSilentMode(YAML::Node &root, bool sm);

bool AddDefaultCommands(YAML::Node &root);

bool ExtractTemplate(const std::filesystem::path &path, YAML::Node &root,
                     const std::string &name, const std::string &version,
                     const std::string &author, const std::string &description,
                     const CommandUtils::CommandMode &cm, bool sm);
} // namespace Extractor