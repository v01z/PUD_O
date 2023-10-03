#include "PackageUseDirHandler.h"
#include <filesystem>
#include <fstream>
#include <iostream>

//-----------------------------------------------------------------

PackageUseDirHandler::PackageUseDirHandler() {

  makeBackup();

  std::vector<std::string> configsPaths;
  getFilesPaths(configsPaths, PACKAGE_USE_DIR);
  for (const auto &path : configsPaths)
    updatePackageHolder(getFileBuff(path));

  packagesHolder.sort();

  for (size_t i{}; i < packagesHolder.getPackages().size(); ++i)
    std::cout << packagesHolder.getPackages().at(i).getFullPackageInfoStr()
              << std::endl;
  std::cout << "Size is: " << packagesHolder.getPackages().size() << std::endl;
}

//-----------------------------------------------------------------

bool PackageUseDirHandler::makeBackup() const {
  auto err = std::error_code{};
  std::filesystem::copy(PACKAGE_USE_DIR, TEMP_DIR,
                        std::filesystem::copy_options::recursive |
                            std::filesystem::copy_options::update_existing,
                        err);
  if (err) {
    std::cout << err.message() << '\n'; // debug?
    return false;
  } else
    std::cout << "Backup of " << PACKAGE_USE_DIR << " created on " <<
        TEMP_DIR << '\n';
  return true;
}

//-----------------------------------------------------------------

const std::vector<unsigned char>
PackageUseDirHandler::getFileBuff(const std::string &fileName) const {
  std::vector<unsigned char> retValVec{};
  std::ifstream file(fileName, std::ios::binary);

  if (!file.is_open())
    return retValVec;

  file.seekg(0, std::ios_base::end);
  auto file_length = file.tellg();
  file.seekg(0, std::ios_base::beg);
  retValVec.resize(static_cast<size_t>(file_length));

  file.read(reinterpret_cast<char *>(retValVec.data()), file_length);
  auto success = !file.fail() && file_length == file.gcount();
  file.close();

  if (!success)
    return std::vector<unsigned char>{};

  return retValVec;
}

//-----------------------------------------------------------------

void PackageUseDirHandler::updatePackageHolder(
    const std::vector<unsigned char> &uCharsBuff) {
  if (uCharsBuff.empty())
    return;

  std::string tempStr{};
  std::stringstream ss(std::string(uCharsBuff.begin(), uCharsBuff.end()));
  while (std::getline(ss, tempStr, '\n')) {
    packagesHolder.addPackage(tempStr);
  }
}

//-----------------------------------------------------------------

void PackageUseDirHandler::getFilesPaths(std::vector<std::string> &pathsVec,
                                         const std::string &currentDir) const {

  for (const auto &file : std::filesystem::directory_iterator(currentDir)) {
    if (std::filesystem::is_directory(file)) {
      getFilesPaths(pathsVec, file.path().string());
    } else {
      pathsVec.push_back(file.path().string());
    }
  }
}

//-----------------------------------------------------------------
