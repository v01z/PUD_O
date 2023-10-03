#include "PackagesHolder.h"
#include <algorithm>
#include <iostream>
#include <regex>

//-----------------------------------------------------------------

const std::set<std::string>
PackagesHolder::getSetFromString(const std::string &inputStr) const {

  std::set<std::string> retValSet{};
  if (inputStr.empty() ||
      std::all_of(inputStr.begin(), inputStr.end(),
                  [](unsigned char ch) { return std::isspace(ch); }))
    return retValSet;

  auto by_space = [](unsigned char a, unsigned char b) {
    return std::isspace(a) and std::isspace(b);
  };

  std::string noSpaceTrashesStr = inputStr;

  noSpaceTrashesStr.erase(std::unique(std::begin(noSpaceTrashesStr),
                                      std::end(noSpaceTrashesStr), by_space),
                          std::end(noSpaceTrashesStr));
  if (std::size(noSpaceTrashesStr) && std::isspace(noSpaceTrashesStr.back()))
    noSpaceTrashesStr.pop_back();

  if (std::size(noSpaceTrashesStr) && std::isspace(noSpaceTrashesStr.front()))
    noSpaceTrashesStr.erase(0, 1);

  std::stringstream ss(noSpaceTrashesStr);
  std::string oneUseFlagSTr{};

  while (std::getline(ss, oneUseFlagSTr, ' ')) {
    retValSet.emplace(oneUseFlagSTr);
  }

  return retValSet;
}

//-----------------------------------------------------------------

const std::string
PackagesHolder::getStrWithoutFrontPrefix(const std::string &strWithPrefix,
                                         int length) {
  if (!std::size(strWithPrefix))
    return strWithPrefix;

  std::string retValStr{strWithPrefix};

  if (length == 1 && (retValStr[0] == '-' || retValStr[0] == ':'))
    return retValStr.erase(0, 1);
  else if (length == 2 && retValStr.substr(0, 2) == "::")
    return retValStr.erase(0, 2);
  else
    return retValStr;
}

//-----------------------------------------------------------------

bool PackagesHolder::addPackage(const std::string &inputStr) {

  static const std::string COMPARE_REGEXP{"^([<=>]{0,2})"};
  static const std::string CATEGORY_REGEXP{"([A-Za-z0-9+_.-]+){1}"};
  static const std::string DELIMETER_REGEXP{"(/){1}"};
  static const std::string PACKAGE_NAME_REGEXP{
      "(([A-Za-z0-9+_]+)(-[^0-9][A-Za-z0-9+_]+)*){1}"};
  static const std::string VERSION_REGEXP{"(-[A-Za-z0-9+_.-]+)?"};
  static const std::string SLOT_REGEXP{"(:[A-Za-z0-9+_.-]+)?"};
  static const std::string REPOSITORY_REGEXP{"(::[A-Za-z0-9_-]+)?"};
  static const std::string USE_FLAGS_REGEXP{"([\\s[A-Za-z0-9+_@-]*]*)"};
  static const std::string COMMENT_N_END_REGEXP{"[\\s#[A-Za-z0-9+_.-]*]*$"};

  static const std::string WHOLE_PACKAGENAME_PATTERN_REGEXP{
      COMPARE_REGEXP + CATEGORY_REGEXP + DELIMETER_REGEXP +
      PACKAGE_NAME_REGEXP + VERSION_REGEXP + SLOT_REGEXP + REPOSITORY_REGEXP +
      USE_FLAGS_REGEXP + COMMENT_N_END_REGEXP};

  auto rx = std::regex{WHOLE_PACKAGENAME_PATTERN_REGEXP};
  std::smatch results;
  auto matched = std::regex_match(inputStr, results, rx);

  if (matched) {
    packages_.push_back({
        PackageVersion(getStrWithoutFrontPrefix(results[7], 1)), /* version_ */
        results[1], /* compareSigns_ */
        results[2], /* category_ */
        // results[3], /* delimeter */
        results[4],                              /* name_ */
        getStrWithoutFrontPrefix(results[8], 1), /* slot_ */
        getStrWithoutFrontPrefix(results[9], 2), /* repository */
        getSetFromString(results[10])            /* use - flags */
    });

    return true;

  } else
    return false;
}

//-----------------------------------------------------------------

const std::vector<Package> PackagesHolder::getPackages() const {
  return packages_;
}

//-----------------------------------------------------------------

void PackagesHolder::sort() {
  std::sort(packages_.begin(), packages_.end(),
            [](const Package &a, const Package &b) {
              return (a.getCategory() + a.getName() + a.getVersion().value() +
                      a.getSlot()) < (b.getCategory() + b.getName() +
                                      b.getVersion().value() + b.getSlot());
            });
}

//-----------------------------------------------------------------
