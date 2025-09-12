//project.hpp
#include "../include/project.hpp"
#include <iostream>
#include "../include/file_utils.hpp"
#include "content_file.hpp"
#include "template_engine.hpp"

void Project::pairReplacers() {
    for (auto& replacer : replacers) {
        const std::string& type = replacer.GetType();
        
        if (type == "PROJECT_NAME") {
            replacer.SetValue(this->name);
        } else if (type == "AUTHOR_NAME") {
            replacer.SetValue(this->author);
        } else if (type == "DATE") {
            replacer.SetValue(this->date);
        } else {
            std::cerr << "Unknown replacer type: " << replacer.GetType() << "\n";
        }
    }
}

void Project::createContentFiles() {
    for (auto& fbp : this->tmpl.GetFileBluePrints()) {
        contentFiles.emplace_back(
            ContentFile(
                Content(
                    TemplateEngine::fillContent(
                        FileUtils::LoadFileToString(fbp.template_path),
                        this->replacers
                    )
                ),
                fbp.target_path
            )
        );
    }
}