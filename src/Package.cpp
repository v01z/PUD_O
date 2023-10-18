#include "Package.h"
#include <algorithm>
#include <iostream>

//-----------------------------------------------------------------

Package::Package(const PackageVersion &version, const std::string &compareSigns,
                 const std::string &category, const std::string &name,
                 const std::string &slot, const std::string &repository,
                 const std::set<std::string> &uses)
    : version_{version}, compareSigns_{compareSigns}, category_{category},
      name_{name}, slot_{slot}, repository_{repository}, useFlags_{uses} {}

//-----------------------------------------------------------------

void Package::setUseFlags(const std::set<std::string> &useFlags) {
  useFlags_ = useFlags;
}

//-----------------------------------------------------------------

const std::set<std::string> &Package::getUseFlags() const { return useFlags_; }
//-----------------------------------------------------------------

const std::string &Package::getCompareSigns() const { return compareSigns_; }

//-----------------------------------------------------------------

const std::string &Package::getCategory() const { return category_; }

//-----------------------------------------------------------------

const std::string &Package::getName() const { return name_; }

//-----------------------------------------------------------------

const PackageVersion &Package::getVersion() const { return version_; }

//-----------------------------------------------------------------

const std::string &Package::getSlot() const { return slot_; }

//-----------------------------------------------------------------

const std::string &Package::getRepository() const { return repository_; }

//-----------------------------------------------------------------

bool Package::isBasicallyTheSame(const Package &other) const {
  /*
 assert(std::ranges::is_sorted(useFlags_) &&
        std::ranges::is_sorted(other.useFlags_));
*/

  return category_ == other.category_ && name_ == other.name_ &&
         slot_ == other.slot_ && repository_ == other.repository_ &&
         useFlags_ == other.useFlags_; // ?
}

//-----------------------------------------------------------------

bool Package::operator==(const Package &other) const {
  return isBasicallyTheSame(other) && compareSigns_ == other.compareSigns_ &&
         version_ == other.version_;
}

bool Package::operator>(const Package &other) const {
  if (isBasicallyTheSame(other)) {
    // TOD0: compareSigns?
    return this->version_ > other.version_;
  }
  return false;
}

//-----------------------------------------------------------------

bool Package::operator<(const Package &other) const {
  if (isBasicallyTheSame(other)) {
    // TOD0: compareSigns?
    return this->version_ < other.version_;
  }
  return false;
}

//-----------------------------------------------------------------

const std::string Package::getFullPackageInfoStr() const {

  std::string usesStr{};
  // consider using std::accumulate instead of a raw loop
  /*
    using namespace std::string_literals;
    std::string usesStr= std::accumulate(str_vec.begin(), str_vec.end(),
    ""s,
        [](const auto& init, const auto& next){
            return init + " " + next;
        });
*/
  for (const auto &useFlag : useFlags_)
    usesStr += " " + useFlag;

  return compareSigns_ + category_ + '/' + name_ +
         (version_.value().empty() ? "" : "-") + version_.value() +
         (slot_.empty() ? "" : ":") + slot_ +
         (repository_.empty() ? "" : "::") + repository_ + usesStr;
}

//-----------------------------------------------------------------

void Package::debugPrintPackage() const {
  std::cout << "\nPackage: " << getFullPackageInfoStr() << '\n';
  std::cout << "Compare sign(s): " << compareSigns_ << '\n';
  std::cout << "Category: " << category_ << '\n';
  std::cout << "Name: " << name_ << '\n';
  std::cout << "Version: " << version_.value() << '\n';
  std::cout << "Slot: " << slot_ << '\n';
  std::cout << "Repository: " << repository_ << '\n';

  std::cout << "USE flags: ";
  std::ranges::for_each(useFlags_, [](auto &&use) { std::cout << use << ' '; });
  std::cout << "\nCount of use flags: " << useFlags_.size() << std::endl;
}

//-----------------------------------------------------------------

Package &Package::operator=(const Package &other) {
  if(this == &other)
    return *this;

  version_ = other.version_;
  compareSigns_ = other.compareSigns_;
  category_ = other.category_;
  name_ = other.name_;
  slot_ = other.slot_;
  repository_ = other.repository_;
  useFlags_ = other.useFlags_;

  return *this;
}

//-----------------------------------------------------------------
