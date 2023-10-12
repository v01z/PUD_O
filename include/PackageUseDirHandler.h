#ifndef PACKAGEUSEDIRHANDLER_H
#define PACKAGEUSEDIRHANDLER_H

#include "PackagesHolder.h"
#include <filesystem>

//-----------------------------------------------------------------

class PackageUseDirHandler {
  const std::filesystem::path PACKAGE_USE_DIR{"/home/knight/temp/package.use" };
  const std::filesystem::path TEMP_DIR_STR{"/tmp/backup_of_package_use"};

  std::vector<std::filesystem::path> filesToExclude_;
  PackagesHolder packagesHolder_;

  void makeBackup() const;

  const std::vector<unsigned char> getFileBuff(const std::filesystem::path&) const;
  void updatePackageHolder(const std::vector<unsigned char> &);
  void getFilesPaths(std::vector<std::filesystem::path> &, const std::filesystem::path&) const;
  void removeFile(const std::string&) const;
  void correctExcludeFilesPaths();
  void generateNewConfigFiles(const std::vector<std::filesystem::path>&) const;

public:
  PackageUseDirHandler() = delete;
  PackageUseDirHandler(const std::vector<std::filesystem::path>&);
  const PackagesHolder &getPackagesHolder() const;
};

//-----------------------------------------------------------------

#endif // PACKAGEUSEDIRHANDLER_H
