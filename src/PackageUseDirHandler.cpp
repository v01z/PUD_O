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
    fillFilesPaths(configsPaths, PACKAGE_USE_DIR);
    for (const auto &path : configsPaths) {
      updatePackageHolder(getFileBuff(path));
      removeFile(path);
    }

    packagesHolder_.sort();
    //debug:
    generateNewConfigFiles();
    //end debug
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

void PackageUseDirHandler::fillFilesPaths(std::vector<std::filesystem::path> &pathsVec,
  const std::filesystem::path &currentDir) const {

    for (const auto &file : std::filesystem::directory_iterator(currentDir)) {
    if (std::filesystem::is_directory(file)) {
      fillFilesPaths(pathsVec, file.path());
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

void PackageUseDirHandler::generateNewConfigFiles() const {
  // continue here
  //debug
  /*
  for (const auto elem : packagesHolder_.getPackages())
  {
    //std::cout << getNewConfigPath(elem.getCategory()) << std::endl;
    std::filesystem::path currentPath = getNewConfigPath(elem.getCategory());
  }
   */

  std::filesystem::path currentPath = getNewConfigPath(packagesHolder_.getPackages().at(0).getCategory());
  std::cout << currentPath << '\n';
  size_t countOfFiles{ 1 };

  for(size_t i{ 1 }; i < packagesHolder_.getPackages().size(); ++i)
  {
    std::filesystem::path nextPath = getNewConfigPath(packagesHolder_.getPackages().at(i).getCategory());
    if(nextPath != currentPath)
    {
      currentPath = nextPath;
      ++countOfFiles;
      std::cout << currentPath << '\n';
    }
  }
  std::cout << "\nCount of files: " << countOfFiles << std::endl;
  //end debug
}

//-----------------------------------------------------------------

const std::filesystem::path
PackageUseDirHandler::getNewConfigPath(const std::string &categoryStr) const {

  std::string tempStr { categoryStr };
  std::replace(tempStr.begin(), tempStr.end(), '-', '_');

  return std::filesystem::path{ PACKAGE_USE_DIR / tempStr };
}

//-----------------------------------------------------------------
