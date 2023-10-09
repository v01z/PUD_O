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

  //

  try {
    PackageUseDirHandler packageUseDirHandler{"/home/knight/temp/package.use/steam",
      "/home/knight/temp/package.use/wine"};
    debugShowPackages(packageUseDirHandler);
  }
  catch(std::exception &exc)
  {
    std::cout << exc.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
