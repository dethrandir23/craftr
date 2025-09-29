#include "../include/extractor.hpp" // Header'ı bu şekilde eklemek daha standarttır
#include "../include/command_utils.hpp"
#include "../include/file_blueprint.hpp"
#include "../include/file_utils.hpp"
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/yaml.h> // Ana yaml.h header'ını dahil etmek genellikle yeterlidir

namespace Extractor {

bool ExtractFiles(const std::filesystem::path &directory,
                  std::vector<FileBlueprint> &fbp) {
  try {
    if (!std::filesystem::exists(directory) ||
        !std::filesystem::is_directory(directory)) {
      throw std::runtime_error(
          "Entry path does not exist or is not a directory!");
    }
    for (const auto &entry :
         std::filesystem::recursive_directory_iterator(directory)) {
      if (!entry.is_regular_file())
        continue;

      auto rel = std::filesystem::relative(entry.path(), directory);
      if (entry.path().stem().string() == "LICENSE") {
        FileBlueprint fb(rel, "license");
        fb.origin_path = entry.path();
        fbp.emplace_back(fb);
        continue;
      }
      FileBlueprint fb(rel, rel.string() + ".txt");
      fb.origin_path = entry.path();
      fbp.emplace_back(fb);
    }

  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
  return true;
}

bool ExtractSubfolders(const std::filesystem::path &path,
                       std::vector<std::filesystem::path> &sf) {
  try {
    if (!std::filesystem::exists(path) ||
        !std::filesystem::is_directory(path)) {
      throw std::runtime_error("Path is not a valid directory!");
    }

    for (const auto &entry :
         std::filesystem::recursive_directory_iterator(path)) {
      if (entry.is_directory()) {
        auto rel = std::filesystem::relative(entry.path(), path);
        if (!rel.empty()) {
          sf.emplace_back(rel);
        }
      }
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
  return true;
}

bool CopyFileTemplates(const std::filesystem::path &dst,
                       const std::vector<FileBlueprint> &fbp) {
  try {
    for (const auto &entry : fbp) {
      auto full_src = entry.origin_path;
      auto full_dst = dst / (entry.template_path);

      if (!full_dst.parent_path().empty()) {
        std::filesystem::create_directories(full_dst.parent_path());
      }

      std::filesystem::copy_file(
          full_src, full_dst,
          std::filesystem::copy_options::overwrite_existing);
    }
  } catch (const std::filesystem::filesystem_error &e) {
    std::cerr << "File copy error: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool AddFiles(YAML::Node &root, const std::vector<FileBlueprint> &fbp) {
  try {
    YAML::Node filesNode = root["files"];
    for (const auto &f : fbp) {
      YAML::Node file;
      file["target"] = f.target_path.string();
      file["template"] = ("files" / f.template_path).string();
      filesNode.push_back(file);
    }
  } catch (const std::exception &e) {
    std::cerr << "YAML AddFiles error: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool AddSubfolders(YAML::Node &root,
                   const std::vector<std::filesystem::path> &sf) {
  try {
    YAML::Node subfoldersNode = root["subfolders"];
    for (const auto &s : sf) {
      subfoldersNode.push_back(s.string());
    }
  } catch (const std::exception &e) {
    std::cerr << "YAML AddSubfolders error: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool AddMetadata(YAML::Node &root, const std::string &name,
                 const std::string &version, const std::string &author,
                 const std::string &description) {
  try {
    root["name"] = name;
    root["version"] = version;
    root["author"] = author;
    root["description"] = description;
  } catch (const std::exception &e) {
    std::cout << "YAML AddMetadata error: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool AddDefaultVariables(YAML::Node &root) {
  try {
    YAML::Node variablesNode(YAML::NodeType::Map);

    variablesNode["PROJECT_NAME"] = "{{PROJECT_NAME}}";
    variablesNode["AUTHOR_NAME"] = "{{AUTHOR_NAME}}";
    variablesNode["DATE"] = "{{DATE}}";

    root["variables"] = variablesNode;
  } catch (const std::exception &e) {
    std::cout << "YAML AddDefaultVariables error: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool AddCommandMode(YAML::Node &root, const CommandUtils::CommandMode &cm) {
  try {
    if (cm == CommandUtils::CommandMode::Cautious) {
      root["command_mode"] = "cautious";
    } else {
      root["command_mode"] = "execute_all";
    }
  } catch (const std::exception &e) {
    std::cout << "YAML AddCommandMode error: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool AddSilentMode(YAML::Node &root, bool sm) {
  try {
    root["silent_mode"] = sm;
  } catch (const std::exception &e) {
    std::cout << "YAML AddSilentMode error: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool AddDefaultCommands(YAML::Node &root) {
  try {
    YAML::Node commandsNode = root["commands"];
    YAML::Node cm1, cm2, cm3;
    cm1["command"] = "cd {{PROJECT_NAME}} && git init";
    cm1["description"] = "Initializing Git repository for {{PROJECT_NAME}}...";
    cm2["command"] = "cd {{PROJECT_NAME}} && git add .";
    cm2["description"] = "Staging files for first commit...";
    cm3["command"] = "cd {{PROJECT_NAME}} && git commit -m 'Initial commit'";
    cm3["description"] = "Creating first commit...";

    commandsNode.push_back(cm1);
    commandsNode.push_back(cm2);
    commandsNode.push_back(cm3);
  } catch (const std::exception &e) {
    std::cout << "YAML AddDefaultCommands error: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool ExtractTemplate(const std::filesystem::path &path, YAML::Node &root,
                     const std::string &name, const std::string &version,
                     const std::string &author, const std::string &description,
                     const CommandUtils::CommandMode &cm, bool sm) {
  try {
    if (!std::filesystem::exists(path) ||
        !std::filesystem::is_directory(path)) {
      throw std::runtime_error("Path is not a valid directory!");
    }

    // Metadata
    if (!AddMetadata(root, name, version, author, description))
      return false;

    // Files & subfolders
    std::vector<FileBlueprint> fbp;
    std::vector<std::filesystem::path> sf;

    if (!ExtractFiles(path, fbp))
      return false;
    if (!ExtractSubfolders(path, sf))
      return false;
    if (!AddFiles(root, fbp))
      return false;
    if (!AddSubfolders(root, sf))
      return false;

    // Variables, modes, commands
    if (!AddDefaultVariables(root))
      return false;
    if (!AddCommandMode(root, cm))
      return false;
    if (!AddSilentMode(root, sm))
      return false;
    if (!AddDefaultCommands(root))
      return false;

    auto dest = FileUtils::get_templates_folder() / "user" / name;
    auto yaml_dest = dest / (name + ".yaml");
    auto file_dest = dest / "files";
    std::filesystem::create_directories(file_dest);

    if (!CopyFileTemplates(file_dest, fbp)) {
      std::cerr << "Failed to copy file templates." << std::endl;
      return false;
    }

    YAML::Emitter out;
    out << root;

    std::ofstream fout(yaml_dest);
    if (!fout.is_open()) {
      std::cerr << "Failed to open output file: " << yaml_dest << std::endl;
      return false;
    }
    fout << out.c_str();
    fout.close();

  } catch (const std::exception &e) {
    std::cout << "ExtractTemplate error: " << e.what() << std::endl;
    return false;
  }

  return true;
}

} // namespace Extractor