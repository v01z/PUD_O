#include "PackageUseDirHandler.h"
#include <algorithm>
#include <fstream>
#include <iostream>

//-----------------------------------------------------------------

PackageUseDirHandler::PackageUseDirHandler(
    const std::vector<std::filesystem::path> &excludeList)
    : filesToExclude_{excludeList}, packagesHolder_{} {

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
    // debug:
    generateNewConfigFiles();
    // end debug
  } catch (...) {
    throw;
  }
}

//-----------------------------------------------------------------

void PackageUseDirHandler::makeBackup() const {
  try {
    std::filesystem::copy(PACKAGE_USE_DIR, TEMP_DIR_STR,
                          std::filesystem::copy_options::recursive |
                              std::filesystem::copy_options::update_existing);

    std::cout << "\nBackup of configs from " << PACKAGE_USE_DIR
              << " created in " << TEMP_DIR_STR << ".\n";
  } catch (...) {
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

void PackageUseDirHandler::fillFilesPaths(
    std::vector<std::filesystem::path> &pathsVec,
    const std::filesystem::path &currentDir) const {

  for (const auto &file : std::filesystem::directory_iterator(currentDir)) {
    if (std::filesystem::is_directory(file)) {
      fillFilesPaths(pathsVec, file.path());
    } else {
      auto filePathStr = file.path().string();

      if (std::find(filesToExclude_.begin(), filesToExclude_.end(),
                    filePathStr) == filesToExclude_.end()) {
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

void PackageUseDirHandler::removeFile(const std::string &fileToRemove) const {
  try {
    std::filesystem::remove(fileToRemove);
  } catch (...) {
    throw;
  }
}

//-----------------------------------------------------------------

void PackageUseDirHandler::correctExcludeFilesPaths() {

  for (auto &path : filesToExclude_) {
    if (path.has_parent_path() && path.parent_path() == PACKAGE_USE_DIR)
      continue;

    path = PACKAGE_USE_DIR.string() / path.filename();
  }
}

//-----------------------------------------------------------------

void PackageUseDirHandler::generateNewConfigFiles() const {

  // assert packagesHolder_.getPackages() is sorted

  Package currentPackage = packagesHolder_.getPackages().at(0);
  std::filesystem::path currentPath =
      getNewConfigPath(currentPackage.getCategory());
  std::string currentPackageStr = currentPackage.getFullPackageInfoStr() + '\n';
  currentPackageStr.insert(0, "#\n# ******* " + currentPackage.getCategory() + " ********\n#\n");

  std::cout << "\n**************\n";
  std::cout << currentPath << '\n';
  std::cout << currentPackageStr << '\n';
  size_t countOfFiles{1};


  std::ofstream currentFile(currentPath.string(),
                            std::ios::app | std::ios::binary);

  if (currentFile.is_open()) {
    std::cout << "\nStart: Writing to " << currentPath << std::endl;
    currentFile.write(reinterpret_cast<char *>(currentPackageStr.data()),
                      currentPackageStr.size());
  }

  Package previousPackage = currentPackage; // needs to implement operator=

  for (size_t i{ 1 }; i < packagesHolder_.getPackages().size(); ++i) {

    currentPackage = packagesHolder_.getPackages().at(i);

    if(getFirstPartOfCategory(currentPackage.getCategory()) !=
                               getFirstPartOfCategory(previousPackage.getCategory()))
    {
      //debug
      std::cout << "\n^^^^^^^ First part of category doesnt equal ^^^^^^^^\n" <<
      previousPackage.getFullPackageInfoStr() << " and " << currentPackage.getFullPackageInfoStr()
      << std::endl;

      currentPath = getNewConfigPath(currentPackage.getCategory());
      std::cout << "New path is: " << currentPath << std::endl; //debug

      currentFile.clear();
      currentFile.close();
      currentFile.open(currentPath.string(), std::ios::app | std::ios::binary);

      ++countOfFiles;
    }

    if (currentFile.is_open()) // hmm, do we really need it?
    {
      currentPackageStr = currentPackage.getFullPackageInfoStr() + '\n';

      if(previousPackage.getCategory() != currentPackage.getCategory())
        currentPackageStr.insert(0, "#\n# ******* " + currentPackage.getCategory() + " ********\n#\n");

      // if basicallyTheSame.. add #

      std::cout << countOfFiles << ": Writing to " << currentPath << std::endl;
      currentFile.write(reinterpret_cast<char *>(currentPackageStr.data()),
                        currentPackageStr.size());
    }

    previousPackage = currentPackage;

  }
  std::cout << "\nCount of files: " << countOfFiles << std::endl;
}

//-----------------------------------------------------------------

const std::filesystem::path
PackageUseDirHandler::getNewConfigPath(const std::string &category) const {

  return std::filesystem::path{ PACKAGE_USE_DIR / getFirstPartOfCategory(category)};
}

//-----------------------------------------------------------------

const std::string
PackageUseDirHandler::getFirstPartOfCategory(const std::string &categoryStr) const {

  std::string retValStr{};

  if (std::size_t found = categoryStr.find('-', 0); found != std::string::npos)
    retValStr = categoryStr.substr(0, found);
  else
    retValStr = categoryStr;

  return retValStr;
}

//-----------------------------------------------------------------
