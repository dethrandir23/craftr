// project.hpp
#pragma once

#include "command_utils.hpp"
#include "content_file.hpp"
#include "date_utils.hpp"
#include "project_template.hpp"
#include "replacer.hpp"
#include <filesystem>
#include <string>
#include <vector>

class Project {
private:
  std::string name;
  std::string version;
  std::string author;
  std::string date;
  std::string licenseType;

  std::vector<Replacer> replacers;
  std::vector<ContentFile> contentFiles;
  std::vector<std::filesystem::path> subFolders;

  CommandUtils::CommandMode commandMode;
  bool silent_mode;
  std::vector<std::string> commands;

  std::filesystem::path projectSubFolder;
  ProjectTemplate tmpl;

public:
  Project(const ProjectTemplate &t) : tmpl(t) {
    date = DateUtils::GetCurrentYearStr();
    replacers = t.GetReplacers();
    subFolders = t.GetSubFolders();
    commands = t.getCommands();
    commandMode = t.getCommandMode();
    silent_mode = t.getSilentMode();
  }

  const std::string &getName() const { return name; }
  const std::string &getVersion() const { return version; }
  const std::string &getAuthor() const { return author; }
  const std::string &getDate() const { return date; }
  const std::string &getLicenseType() const { return licenseType; }

  const std::vector<ContentFile> &getContentFiles() const {
    return contentFiles;
  }
  const std::vector<std::filesystem::path> &getSubFolders() const {
    return subFolders;
  }

  const ProjectTemplate &getTemplate() const { return tmpl; }
  const std::filesystem::path &getProjectSubFolder() const {
    return projectSubFolder;
  }

  std::vector<Replacer> &getReplacers() { return this->replacers; }
  std::vector<std::string> &getCommands() { return this->commands; }

  void setName(const std::string &n) { name = n; }
  void setVersion(const std::string &v) { version = v; }
  void setAuthor(const std::string &a) { author = a; }
  void setDate(const std::string &d) { date = d; }
  void setLicenseType(const std::string &l) { licenseType = l; }

  void setContentFiles(const std::vector<ContentFile> &cfs) {
    contentFiles = cfs;
  }
  void setSubFolders(const std::vector<std::filesystem::path> s) {
    subFolders = s;
  }

  void setTemplate(const ProjectTemplate &t) { tmpl = t; }
  void setProjectSubFolder(const std::filesystem::path &p) {
    projectSubFolder = p;
  }

  CommandUtils::CommandMode getCommandMode() const { return this->commandMode; }
  void setCommandMode(const CommandUtils::CommandMode &cm) {
    this->commandMode = cm;
  }

  void setCommands(const std::vector<std::string> &c) { this->commands = c; }

  bool getSilentMode() const { return this->silent_mode; }
  void setSilentMode(bool sm) { this->silent_mode = sm; }

  void createCommands();
  void createContentFiles();
};
