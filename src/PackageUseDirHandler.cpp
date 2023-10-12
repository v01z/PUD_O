#include "PackageUseDirHandler.h"
#include <fstream>
#include <algorithm>
#include <iostream>

//-----------------------------------------------------------------

PackageUseDirHandler::PackageUseDirHandler(const std::vector<std::filesystem::path> &excludeList):
  filesToExclude_{ excludeList }, packagesHolder_{}
{

  try {
    correctExcludeFilesPaths();
    makeBackup();

    std::vector<std::filesystem::path> configsPaths;
    getFilesPaths(configsPaths, PACKAGE_USE_DIR);
    for (const auto &path : configsPaths) {
      updatePackageHolder(getFileBuff(path));
      removeFile(path);
    }

    packagesHolder_.sort();
  }
  catch (...) {
    throw;
  }
}

//-----------------------------------------------------------------

void PackageUseDirHandler::makeBackup() const {
  try {
    std::filesystem::copy(PACKAGE_USE_DIR, TEMP_DIR_STR,
        std::filesystem::copy_options::recursive |
            std::filesystem::copy_options::update_existing);

    std::cout << "\nBackup of configs from " << PACKAGE_USE_DIR << " created in "
      << TEMP_DIR_STR << ".\n";
  }
  catch(...)
  {
    throw;
  }
}

//-----------------------------------------------------------------

const std::vector<unsigned char>
PackageUseDirHandler::getFileBuff(const std::filesystem::path &fileName) const {
  std::vector<unsigned char> retValVec{};
  std::ifstream file(fileName.string(), std::ios::binary);

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
    packagesHolder_.addPackage(tempStr);
  }
}

//-----------------------------------------------------------------

void PackageUseDirHandler::getFilesPaths(std::vector<std::filesystem::path> &pathsVec,
  const std::filesystem::path &currentDir) const {

    for (const auto &file : std::filesystem::directory_iterator(currentDir)) {
    if (std::filesystem::is_directory(file)) {
      getFilesPaths(pathsVec, file.path());
    } else {
      auto filePathStr = file.path().string();

      if(std::find(filesToExclude_.begin(), filesToExclude_.end(),
                    filePathStr) == filesToExclude_.end())
      {
        pathsVec.push_back(filePathStr);
      }
    }
  }
}

//-----------------------------------------------------------------

const PackagesHolder &PackageUseDirHandler::getPackagesHolder() const {
  return packagesHolder_;
}

//-----------------------------------------------------------------

void PackageUseDirHandler::removeFile(const std::string& fileToRemove) const{
  try{
    std::filesystem::remove(fileToRemove);
  }
  catch(...)
  {
    throw;
  }
}

//-----------------------------------------------------------------

void PackageUseDirHandler::correctExcludeFilesPaths() {

  for (auto &path : filesToExclude_)
  {
    if(path.has_parent_path() && path.parent_path() == PACKAGE_USE_DIR)
      continue;

    path = PACKAGE_USE_DIR.string() / path.filename();
  }
}

//-----------------------------------------------------------------

void PackageUseDirHandler::generateNewConfigFiles
    (const std::vector<std::filesystem::path>& configsPaths) const {
  // continue here
}

//-----------------------------------------------------------------
