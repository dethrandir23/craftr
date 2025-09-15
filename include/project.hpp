// project.hpp
#pragma once

#include "content_file.hpp"
#include "date_utils.hpp"
#include "project_template.hpp"
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

  std::filesystem::path projectSubFolder;
  ProjectTemplate tmpl;

public:
  Project(const std::string &n, const std::string &v, const std::string &a,
          const ProjectTemplate &t)
      : name(n), version(v), author(a), tmpl(t) {
    date = DateUtils::GetCurrentYearStr();
    replacers = t.GetReplacers();
    subFolders = t.GetSubFolders();
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

  void createLicenseFile();
  void pairReplacers();
  void createContentFiles();
};
