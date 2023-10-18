#ifndef PACKAGEUSEDIRHANDLER_H
#define PACKAGEUSEDIRHANDLER_H

#include "PackagesHolder.h"
#include <filesystem>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KMAG  "\x1B[35m"

//-----------------------------------------------------------------

class PackageUseDirHandler {
   const std::filesystem::path PACKAGE_USE_DIR{"/home/knight/temp/package.use"};
  //const std::filesystem::path PACKAGE_USE_DIR{"/home/bcat/temp/package.use"};
  const std::filesystem::path TEMP_DIR_STR{"/tmp/backup_of_package_use"};

  std::vector<std::filesystem::path> filesToExclude_;
  PackagesHolder packagesHolder_;

  void makeBackup() const;

  const std::vector<unsigned char>
  getFileBuff(const std::filesystem::path &) const;
  void updatePackageHolder(const std::vector<unsigned char> &);
  void fillFilesPaths(std::vector<std::filesystem::path> &,
                      const std::filesystem::path &) const;
  void removeFile(const std::string &) const;
  void correctExcludeFilesPaths();
  void generateNewConfigFiles() const;
  const std::string getFirstPartOfCategory(const std::string&) const;
  const std::filesystem::path getNewConfigPath(const std::string &) const;
  void printExcludeFilesVec() const;

public:
  PackageUseDirHandler() = delete;
  explicit PackageUseDirHandler(const std::vector<std::filesystem::path> &);
  const PackagesHolder &getPackagesHolder() const;
};

//-----------------------------------------------------------------

#endif // PACKAGEUSEDIRHANDLER_H
