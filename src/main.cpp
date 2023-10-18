#include "PackageUseDirHandler.h"
#include <fstream> //debug
#include <iostream>

//debug
void debugShowPackages(const PackageUseDirHandler &packageUseDirHandler){
  auto pkgs = packageUseDirHandler.getPackagesHolder().getPackages();
  for (size_t i{}; i < pkgs.size(); ++i)
    std::cout << pkgs.at(i).getFullPackageInfoStr()
    << std::endl;
  std::cout << "Size is: " << pkgs.size()
  << std::endl;
}

//debug too
void writeWholeVecToFile(const PackageUseDirHandler &packageUseDirHandler){
  std::ofstream file("/home/knight/temp/wholeVec.txt");
  auto vec = packageUseDirHandler.getPackagesHolder().getPackages();
  for(const auto &pckg: vec)
  {
    file << (pckg.getFullPackageInfoStr() + '\n');
  }
}

//-----------------------------------------------------------------

int main(int argc, char **argv) {

  if (argc == 2 && std::string(argv[1]) == "--help")
  {
    std::cout << "Usage:\n" << argv[0] << std::endl <<
        "or:\n" << argv[0] << " --exclude \'files to exclude\'\n";
    return EXIT_SUCCESS;
  }

  std::vector<std::filesystem::path> argvVec{};

  if(argc > 2 && std::string(argv[1]) == "--exclude") {
      for (size_t i{ 2 }; i < argc; ++i) {
        argvVec.push_back(argv[i]);
      }
  }

  try {
    PackageUseDirHandler packageUseDirHandler{ std::move(argvVec) };
    //debugShowPackages(packageUseDirHandler);
    writeWholeVecToFile(packageUseDirHandler);
  }
  catch(std::exception &exc)
  {
    std::cout << exc.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

//-----------------------------------------------------------------
