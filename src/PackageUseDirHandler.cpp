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

    size_t filesCount{};
    for (const auto &path : configsPaths) {
      updatePackageHolder(getFileBuff(path));
      removeFile(path);
      ++filesCount;
    }

    std::cout << '\n' << filesCount << " file(s) removed.\n";
    printExcludeFilesVec();

    packagesHolder_.sort();
    generateNewConfigFiles();
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
              << " has been created in " << TEMP_DIR_STR <<
        ". You can get them from there if something goes wrong. " <<
            "But do not forget that after rebooting system they will be deleted.\n\n";
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
    std::cout << "File " << KRED << fileToRemove << KNRM << " removed.\n";
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

  // assert (packagesHolder_.getPackages() is sorted)

  Package currentPackage = packagesHolder_.getPackages().at(0);
  Package previousPackage = currentPackage;

  std::filesystem::path currentPath =
      getNewConfigPath(currentPackage.getCategory());
  std::string currentPackageStr = currentPackage.getFullPackageInfoStr() + '\n';

  size_t countOfFiles{1};

  std::cout << "\nCreating file " << KGRN << currentPath << KNRM << '\n';
  std::ofstream currentFile(currentPath.string(),
                            std::ios::app | std::ios::binary);

  if (currentFile.is_open()) {
    currentFile.write(reinterpret_cast<char *>(currentPackageStr.data()),
                      currentPackageStr.size());
  }


  for (size_t i{ 1 }; i < packagesHolder_.getPackages().size(); ++i) {

    currentPackage = packagesHolder_.getPackages().at(i);

    if(getFirstPartOfCategory(currentPackage.getCategory()) !=
                               getFirstPartOfCategory(previousPackage.getCategory()))
    {
      currentPath = getNewConfigPath(currentPackage.getCategory());

      currentFile.clear();
      currentFile.close();

      std::cout << "Creating file " << KGRN << currentPath << KNRM << '\n';
      currentFile.open(currentPath.string(), std::ios::app | std::ios::binary);

      ++countOfFiles;
    }

    if (currentFile.is_open())
    {
      currentPackageStr = currentPackage.getFullPackageInfoStr() + '\n';

      if(previousPackage.getName() != currentPackage.getName())
        currentPackageStr.insert(0, "#\n");

      if(previousPackage.getCategory() != currentPackage.getCategory())
        currentPackageStr.insert(0, "#\n");

      currentFile.write(reinterpret_cast<char *>(currentPackageStr.data()),
                        currentPackageStr.size());
    }

    previousPackage = currentPackage;

  }
  std::cout << '\n' << countOfFiles << " config file(s) created." << std::endl;
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

void PackageUseDirHandler::printExcludeFilesVec() const {
  if(filesToExclude_.empty())
    return;

  std::cout << "Skipped " << filesToExclude_.size() << " file(s):\n";
  for(const auto &fileName : filesToExclude_)
    std::cout << KYEL << fileName << KNRM << '\n';
}

//-----------------------------------------------------------------
