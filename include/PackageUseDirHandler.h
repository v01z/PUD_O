#ifndef PACKAGEUSEDIRHANDLER_H
#define PACKAGEUSEDIRHANDLER_H

#include "PackagesHolder.h"
#include <filesystem>

//-----------------------------------------------------------------

class PackageUseDirHandler {
  const std::string TEMP_DIR_STR{"/tmp/backup_of_package_use"};

  std::vector<std::filesystem::path> filesToExclude_;
  PackagesHolder packagesHolder_;
  const std::filesystem::path PACKAGE_USE_DIR_PATH;

  void makeBackup() const;

  const std::vector<unsigned char> getFileBuff(const std::string &) const;
  void updatePackageHolder(const std::vector<unsigned char> &);
  void getFilesPaths(std::vector<std::string> &, const std::filesystem::path&) const;
  void removeFile(const std::string&);
  void correctExcludeFilesPaths();

public:
  PackageUseDirHandler() = delete;
  PackageUseDirHandler(const std::vector<std::filesystem::path>&,
      // start here. /etc/portage/package.use/make.conf
      const std::filesystem::path& packageUseDir = "/home/knight/temp/package.use");
  const PackagesHolder &getPackagesHolder() const;
};

//-----------------------------------------------------------------

#endif // PACKAGEUSEDIRHANDLER_H
