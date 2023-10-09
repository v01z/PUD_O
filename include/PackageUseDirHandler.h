#ifndef PACKAGEUSEDIRHANDLER_H
#define PACKAGEUSEDIRHANDLER_H

#include "PackagesHolder.h"
#include <filesystem>

//-----------------------------------------------------------------

class PackageUseDirHandler {
  //const std::filesystem::path PACKAGE_USE_DIR_PATH{"/etc/portage/package.use"};
  const std::filesystem::path PACKAGE_USE_DIR_PATH{"/home/knight/temp/package.use"};
  //const std::filesystem::path PACKAGE_USE_DIR_PATH{"~/temp/package.use"};
  const std::string TEMP_DIR_STR{"/tmp/backup_of_package_use"};

  std::set<std::string> filesToExclude_;
  PackagesHolder packagesHolder_;

  void makeBackup() const;

  const std::vector<unsigned char> getFileBuff(const std::string &) const;
  void updatePackageHolder(const std::vector<unsigned char> &);
  void getFilesPaths(std::vector<std::string> &, const std::filesystem::path&) const;
  void removeFile(const std::string&);

public:
  PackageUseDirHandler() = delete;
  PackageUseDirHandler(std::initializer_list<std::string>);
  const PackagesHolder &getPackagesHolder() const;
};

//-----------------------------------------------------------------

#endif // PACKAGEUSEDIRHANDLER_H
