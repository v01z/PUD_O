#include "PackageUseDirHandler.h"
#include <iostream>

void debugShowPackages(const PackageUseDirHandler &packageUseDirHandler){
  auto pkgs = packageUseDirHandler.getPackagesHolder().getPackages();
  for (size_t i{}; i < pkgs.size(); ++i)
    std::cout << pkgs.at(i).getFullPackageInfoStr()
    << std::endl;
  std::cout << "Size is: " << pkgs.size()
  << std::endl;
}

int main(int argc, char **argv) {

  if (argc == 2 && std::string(argv[1]) == "--help")
  {
    std::cout << "Usage: " << argv[0] << " \'files to exclude\'\n";
    return EXIT_SUCCESS;
  }

  std::set<std::string> argvSet{};
  if (argc > 1)
  {
    for (size_t i{ 1 }; i < argc; ++i)
    {
      argvSet.insert(argv[i]);
    }
  }

  //argvSet.insert("/home/knight/temp/package.use/steam");

  try {
    PackageUseDirHandler packageUseDirHandler{ std::move(argvSet) };
    debugShowPackages(packageUseDirHandler);
  }
  catch(std::exception &exc)
  {
    std::cout << exc.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
