//file_blueprint.hpp
#pragma once

#include <filesystem>

struct FileBlueprint {
    std::filesystem::path template_path;
    std::filesystem::path target_path;

    FileBlueprint() = default;

    FileBlueprint(const std::filesystem::path& p1, const std::filesystem::path& p2)
        : template_path(p1), target_path(p2) {}
};  