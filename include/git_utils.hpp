// git_utils.hpp

#include <filesystem>
#include <string>

namespace GitUtils {

bool isGitInstalled();
bool runGitCloneOrPull(const std::string &repoUrl,
                       const std::filesystem::path &targetDir);

std::string getRepoNameFromUrl(const std::string &url);
} // namespace GitUtils