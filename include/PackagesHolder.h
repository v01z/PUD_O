#ifndef PACKAGESHOLDER_H
#define PACKAGESHOLDER_H

#include "Package.h"

//-----------------------------------------------------------------

class PackagesHolder {
  std::vector<Package> packages_{};

  const std::set<std::string> getSetFromString(const std::string &) const;
  const std::string getStrWithoutFrontPrefix(const std::string &, int);

public:
  PackagesHolder() = default;

  bool addPackage(const std::string &);

  const std::vector<Package> getPackages() const;

  void sort();
};

//-----------------------------------------------------------------

#endif // PACKAGESHOLDER_H
