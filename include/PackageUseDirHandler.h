#ifndef PACKAGEUSEDIRHANDLER_H
#define PACKAGEUSEDIRHANDLER_H

#include "PackagesHolder.h"

//-----------------------------------------------------------------

class PackageUseDirHandler {
  //const std::string PACKAGE_USE_DIR{"/etc/portage/package.use"};
  const std::string PACKAGE_USE_DIR{"~/temp/package.use"};
  const std::string TEMP_DIR{"/tmp/backup_of_package_use"};

  PackagesHolder packagesHolder;

  bool makeBackup() const;

  const std::vector<unsigned char> getFileBuff(const std::string &) const;
  void updatePackageHolder(const std::vector<unsigned char> &);
  void getFilesPaths(std::vector<std::string> &, const std::string &) const;

public:
  PackageUseDirHandler();
};

//-----------------------------------------------------------------

#endif // PACKAGEUSEDIRHANDLER_H
