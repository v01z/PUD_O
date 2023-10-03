#include "PackageVersion.h"
#include <cassert>
#include <regex>

//-----------------------------------------------------------------

const std::string &PackageVersion::value() const { return value_; }

//-----------------------------------------------------------------

void PackageVersion::setValue(const std::string &newVersion) {
  value_ = newVersion;
}

//-----------------------------------------------------------------

bool PackageVersion::operator==(const PackageVersion &other) const {
  return value_ == other.value_;
}

//-----------------------------------------------------------------

bool PackageVersion::operator>(const PackageVersion &other) const {

  if (*this == other)
    return false;

  if (value_.empty() && !other.value_.empty())
    return true;
  if (!value_.empty() && other.value_.empty())
    return false;

  std::vector<std::string> thisVecComponents{getVersionComponents()};
  std::vector<std::string> otherVecComponents{other.getVersionComponents()};

  int maxComponentsCountToTest =
      std::min(thisVecComponents.size(), otherVecComponents.size());

  for (int i{}; i < maxComponentsCountToTest; i++) {
    if (i > 0) {
      char thisStart = *thisVecComponents.at(i).begin();
      char otherStart = *otherVecComponents.at(i).begin();
      if (thisStart == '-' && otherStart == '.')
        return true;
      if (otherStart == '-' && thisStart == '.')
        return false;
      thisVecComponents.at(i).erase(0, 1);
      otherVecComponents.at(i).erase(0, 1);
    }

    int thisCurrentComponent = getIntFromStr(thisVecComponents.at(i));
    int otherCurrentComponent = getIntFromStr(otherVecComponents.at(i));

    if ((thisCurrentComponent == -2) || (otherCurrentComponent == -2)) {
      return false;
    }
    if (thisCurrentComponent == -1 && otherCurrentComponent != -1) {
      return false;
    }
    if (thisCurrentComponent != -1 && otherCurrentComponent == -1) {
      return true;
    }
    if (thisCurrentComponent == -1 && otherCurrentComponent == -1) {

      auto thisCurrntCompVec = getIntsFromStr(thisVecComponents.at(i));
      auto otherCurrntCompVec = getIntsFromStr(otherVecComponents.at(i));
      int maxNumsToTest =
          std::min(thisCurrntCompVec.size(), otherCurrntCompVec.size());
      for (int j{}; j < maxNumsToTest; j++) {
        if (thisCurrntCompVec.at(j) > otherCurrntCompVec.at(j)) {
          return true;
        } else if (thisCurrntCompVec.at(j) < otherCurrntCompVec.at(j)) {
          return false;
        }
      }
      continue;
    }

    if (thisCurrentComponent < otherCurrentComponent) {
      return false;
    } else if (thisCurrentComponent > otherCurrentComponent) {
      return true;
    }
  }

  return thisVecComponents.size() > otherVecComponents.size();
}

//-----------------------------------------------------------------

bool PackageVersion::operator<(const PackageVersion &other) const {

  if (*this == other)
    return false;

  if (value_.empty() && !other.value_.empty())
    return false;
  if (!value_.empty() && other.value_.empty())
    return true;

  std::vector<std::string> thisVecComponents{getVersionComponents()};
  std::vector<std::string> otherVecComponents{other.getVersionComponents()};

  int maxComponentsCountToTest =
      std::min(thisVecComponents.size(), otherVecComponents.size());

  for (int i{}; i < maxComponentsCountToTest; i++) {
    if (i > 0) {
      char thisStart = *thisVecComponents.at(i).begin();
      char otherStart = *otherVecComponents.at(i).begin();
      if (thisStart == '-' && otherStart == '.')
        return false;
      if (otherStart == '-' && thisStart == '.')
        return true;
      thisVecComponents.at(i).erase(0, 1);
      otherVecComponents.at(i).erase(0, 1);
    }
    int thisCurrentComponent = getIntFromStr(thisVecComponents.at(i));
    int otherCurrentComponent = getIntFromStr(otherVecComponents.at(i));

    if ((thisCurrentComponent == -2) || (otherCurrentComponent == -2)) {
      return false;
    }
    if (thisCurrentComponent == -1 && otherCurrentComponent != -1) {
      return true;
    }
    if (thisCurrentComponent != -1 && otherCurrentComponent == -1) {
      return false;
    }
    if (thisCurrentComponent == -1 && otherCurrentComponent == -1) {

      auto thisCurrntCompVec = getIntsFromStr(thisVecComponents.at(i));
      auto otherCurrntCompVec = getIntsFromStr(otherVecComponents.at(i));
      int maxNumsToTest =
          std::min(thisCurrntCompVec.size(), otherCurrntCompVec.size());
      for (int j{}; j < maxNumsToTest; j++) {
        if (thisCurrntCompVec.at(j) < otherCurrntCompVec.at(j)) {
          return true;
        } else if (thisCurrntCompVec.at(j) > otherCurrntCompVec.at(j)) {
          return false;
        }
      }
      continue;
    }

    if (thisCurrentComponent > otherCurrentComponent) {
      return false;
    } else if (thisCurrentComponent < otherCurrentComponent) {
      return true;
    }
  }

  return thisVecComponents.size() < otherVecComponents.size();
}

//-----------------------------------------------------------------

bool PackageVersion::operator>=(const PackageVersion &other) const {
  return *this > other || *this == other;
}

//-----------------------------------------------------------------

bool PackageVersion::operator<=(const PackageVersion &other) const {
  return *this < other || *this == other;
}

//-----------------------------------------------------------------

const std::vector<std::string> PackageVersion::getVersionComponents() const {
  auto retValVec = std::vector<std::string>{};
  if (!value_.empty()) {

    assert(isNumber(*value_.begin()));

    auto tempStr = std::string{};
    for (int i{}; i < value_.length(); ++i) {
      char ch = value_.at(i);
      if (ch == '.' || ch == '-') {
        if (!tempStr.empty()) {
          retValVec.push_back(tempStr);
          tempStr.clear();
        }
      }
      tempStr += ch;
    }
    retValVec.push_back(tempStr);
  }

  return retValVec;
}

//-----------------------------------------------------------------

bool PackageVersion::isNumber(char ch) const { return ch > 47 && ch < 58; }

//-----------------------------------------------------------------

const int PackageVersion::getIntFromStr(const std::string &inputStr) const {
  int retVal;

  try {
    retVal = std::stoi(inputStr);
  } catch (const std::invalid_argument &e) {
    retVal = -1;
  } catch (const std::out_of_range &e) {
    retVal = -2;
  }

  return retVal;
}

//-----------------------------------------------------------------

const std::vector<int>
PackageVersion::getIntsFromStr(const std::string &inputStr) const {
  auto retValVec = std::vector<int>{};

  if (inputStr.empty())
    return retValVec;

  std::string numsStr{};
  for (int i{}; i < inputStr.length(); ++i) {
    char ch = inputStr.at(i);
    if (isNumber(ch)) {
      numsStr += ch;
    } else {
      if (numsStr.empty())
        retValVec.push_back(ch);
      else {
        int numToStr = getIntFromStr(numsStr);
        if (numToStr == -2)
          numToStr = 0;
        retValVec.push_back(numToStr);
        numsStr.clear();

        retValVec.push_back(ch);
      }
    }
  }

  if (!numsStr.empty()) {
    int lastNum = getIntFromStr(numsStr);
    if (lastNum == -2)
      lastNum = 0;
    retValVec.push_back(lastNum);
  }

  return retValVec;
}

PackageVersion::PackageVersion(const std::string &version) : value_{version} {}

//-----------------------------------------------------------------
