// project.hpp
#pragma once

#include "project_template.hpp"
#include <string>
#include "content_file.hpp"
#include <vector>
#include "date_utils.hpp"

class Project
{
private:
    std::string name;
    std::string version;
    std::string author;
    std::string date;

    std::vector<Replacer> replacers; 
    std::vector<ContentFile> contentFiles;

    ProjectTemplate tmpl;

public:
    Project(const std::string& n,
            const std::string& v,
            const std::string& a,
            const ProjectTemplate& t)
        : name(n), version(v), author(a), tmpl(t) 
        { 
            date = DateUtils::GetCurrentYearStr();
            replacers = t.GetReplacers();
        }

    const std::string& getName() const { return name; }
    const std::string& getVersion() const { return version; }
    const std::string& getAuthor() const { return author; }
    const std::string& getDate() const { return date; }

    const std::vector<ContentFile>& getContentFiles() const { return contentFiles; }

    const ProjectTemplate& getTemplate() const { return tmpl; }

    void setName(const std::string& n) { name = n; }
    void setVersion(const std::string& v) { version = v; }
    void setAuthor(const std::string& a) { author = a; }
    void setDate(const std::string& d) { date = d; }

    void setContentFiles(const std::vector<ContentFile>& cfs) { contentFiles = cfs; }

    void setTemplate(const ProjectTemplate& t) { tmpl = t; }

    void pairReplacers();
    void createContentFiles();

};
