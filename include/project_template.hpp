//project_template.hpp
#pragma once

#include "file_blueprint.hpp"
#include <filesystem>
#include <map>
#include <string>
#include <vector>
#include <replacer.hpp>

class ProjectTemplate {
public:
    ProjectTemplate(const std::filesystem::path& template_path) {
        this->template_path = template_path;
    }

    bool LoadTemplate();

    const std::filesystem::path GetTemplatePath() const { return template_path; }
    const std::string& GetName() const { return this->name; }
    const std::string& GetVersion() const { return this->version; }
    const std::string& GetAuthor() const { return this->author; }
    const std::string& GetDescription() const { return this->description; }
    const std::vector<Replacer>& GetReplacers() const { return this->replacers; }
    const std::vector<std::filesystem::path> GetSubFolders() const { return this->subfolders; }
    const std::vector<FileBlueprint>& GetFileBluePrints() const { return this->file_blueprints; }
    const std::map<std::string, std::string>& GetLicenses() const { return this->licenses; }

    void SetTemplatePath(const std::filesystem::path& path) { template_path = path; }
    void SetName(const std::string& new_name) { name = new_name; }
    void SetVersion(const std::string& new_version) { version = new_version; }
    void SetAuthor(const std::string& new_author) { author = new_author; }
    void SetDescription(const std::string& new_description) { description = new_description; }

    void AddReplacer(const std::string& key, const std::string& value) {
        replacers.emplace_back(Replacer(key, value));
    }

    void AddReplacer(const Replacer& r) {
        replacers.emplace_back(r);
    }

    void ClearReplacers() {
        replacers.clear();
    }

    void SetReplacers(const std::vector<Replacer>& new_replacers) {
        replacers = new_replacers;
    }

    void AddSubFolder(const std::filesystem::path& folder_path) {
        subfolders.push_back(folder_path);
    }

    void ClearSubFolders() {
        subfolders.clear();
    }

    void SetSubFolders(const std::vector<std::filesystem::path>& new_subfolders) {
        subfolders = new_subfolders;
    }

    void AddFileBlueprint(const FileBlueprint& blueprint) {
        file_blueprints.push_back(blueprint);
    }

    void ClearFileBlueprints() {
        file_blueprints.clear();
    }

    void SetFileBlueprints(const std::vector<FileBlueprint>& new_blueprints) {
        file_blueprints = new_blueprints;
    }

    void AddLicense(const std::string& license_name, const std::string& license_text) {
        licenses[license_name] = license_text;
    }

    void RemoveLicense(const std::string& license_name) {
        licenses.erase(license_name);
    }

    void ClearLicenses() {
        licenses.clear();
    }

    void SetLicenses(const std::map<std::string, std::string>& new_licenses) {
        licenses = new_licenses;
    }

private:
    std::string name = "";
    std::string version = "";
    std::string author = "";
    std::string description = "";

    std::vector<Replacer> replacers;

    std::vector<std::filesystem::path> subfolders;
    std::vector<FileBlueprint> file_blueprints;
    std::map<std::string, std::string> licenses;

    std::filesystem::path template_path;
};