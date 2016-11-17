#include "Path.h"

#include <sys/stat.h>

#if X_WINDOWS
#include <direct.h>
#else
#include <unistd.h>
#endif

namespace xEngine {

#if X_WINDOWS
eastl::string Path::separator_ = "\\";
#else
eastl::string Path::separator_ = "/";
#endif

Path Path::GetCurrentDirectory() {
  auto result = getcwd(nullptr, 0);
  eastl::string string(result);
  free(result);
  return string;
}

bool Path::Exist(const eastl::string &path) {
  struct stat status;
  return stat(path.c_str(), &status) == 0;
}

bool Path::IsDirectory(const eastl::string &path) {
  struct stat status;
  return stat(path.c_str(), &status) == 0 && S_ISDIR(status.st_mode);
}

void Path::CreateFile(const eastl::string &path) {
  auto fd = fopen(path.c_str(), "w");
  if (fd) fclose(fd);
}

void Path::CreateDirectory(const eastl::string &path) {
  mkdir(path.c_str(), 0644);
}

void Path::Delete(const eastl::string &path) {
  if (Exist(path)) {
    if (IsDirectory(path)) {
      rmdir(path.c_str());
    } else {
      remove(path.c_str());
    }
  }
}

void Path::Move(const eastl::string &from, const eastl::string &to) {
  rename(from.c_str(), to.c_str());
}

Path &Path::ParentDirectory() {
  string_ = string_.substr(0, string_.find_last_of(separator_));
  return *this;
}

Path &Path::Append(const eastl::string &name) {
  string_.append(separator_).append(name);
  return *this;
}

}