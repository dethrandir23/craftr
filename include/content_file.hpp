// content_file.hpp
#pragma once
#include "content.hpp"
#include <filesystem>

struct ContentFile {

  Content content;
  std::filesystem::path target_path;

  ContentFile(const Content &c, const std::filesystem::path &p)
      : content(c), target_path(p) {}
};