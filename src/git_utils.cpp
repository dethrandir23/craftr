// git_utils.cpp
#include <cstdlib>
#include <filesystem>
#include <string>

namespace GitUtils {
bool isGitInstalled() {
#ifdef _WIN32
  int result = system("git --version >nul 2>&1");
#else
  int result = system("git --version > /dev/null 2>&1");
#endif
  return (result == 0);
}

bool runGitCloneOrPull(const std::string &repoUrl,
                       const std::filesystem::path &targetDir) {
  if (!std::filesystem::exists(targetDir)) {
    std::string cmd = "git clone " + repoUrl + " " + targetDir.string();
    return (system(cmd.c_str()) == 0);
  } else {
    std::string cmd = "cd " + targetDir.string() + " && git pull";
    return (system(cmd.c_str()) == 0);
  }
}

std::string getRepoNameFromUrl(const std::string &url) {
  size_t last_slash_pos = url.find_last_of('/');
  if (last_slash_pos == std::string::npos) {
    return "";
  }

  std::string repo_part = url.substr(last_slash_pos + 1);

  if (repo_part.length() > 4 &&
      repo_part.substr(repo_part.length() - 4) == ".git") {
    return repo_part.substr(0, repo_part.length() - 4);
  }

  return repo_part;
}
} // namespace GitUtils