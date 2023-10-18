#ifndef PACKAGEVERSION_H
#define PACKAGEVERSION_H

#include <string>
#include <vector>

//-----------------------------------------------------------------

class PackageVersion {
private:
  std::string value_{};

  bool isNumber(char) const;
  const std::vector<std::string> getVersionComponents() const;
  const int getIntFromStr(const std::string &) const;
  const std::vector<int> getIntsFromStr(const std::string &) const;

public:
  PackageVersion() = default;
  explicit PackageVersion(const std::string &);

  PackageVersion& operator=(const PackageVersion&);

  const std::string &value() const;
  void setValue(const std::string &newVersion);

  bool operator==(const PackageVersion &) const;
  bool operator>(const PackageVersion &) const;
  bool operator<(const PackageVersion &) const;
  bool operator>=(const PackageVersion &) const;
  bool operator<=(const PackageVersion &) const;
};

//-----------------------------------------------------------------

#endif // PACKAGEVERSION_H
