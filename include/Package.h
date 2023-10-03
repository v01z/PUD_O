#ifndef PACKAGE_USE_CLEANER_PACKAGE_H
#define PACKAGE_USE_CLEANER_PACKAGE_H

#include "PackageVersion.h"
#include <set>
#include <string>

//-----------------------------------------------------------------

class Package {

  PackageVersion version_{};
  std::string compareSigns_{};
  std::string category_{};
  std::string name_{};
  std::string slot_{};
  std::string repository_{};
  std::set<std::string> useFlags_{};

  bool isBasicallyTheSame(const Package &) const;

public:
  Package() = default;

  Package(const PackageVersion &, const std::string &, const std::string &,
          const std::string &, const std::string &, const std::string &,
          const std::set<std::string> &);

  void setUseFlags(const std::set<std::string> &useFlags);

  const std::set<std::string> &getUseFlags() const;
  const std::string &getCompareSigns() const;
  const std::string &getCategory() const;
  const std::string &getName() const;
  const PackageVersion &getVersion() const;
  const std::string &getSlot() const;
  const std::string &getRepository() const;

  bool operator==(const Package &) const;
  bool operator>(const Package &) const;
  bool operator<(const Package &) const;

  const std::string getFullPackageInfoStr() const;

  // func for debug purposes
  void debugPrintPackage() const;
};

//-----------------------------------------------------------------

#endif // PACKAGE_USE_CLEANER_PACKAGE_H
