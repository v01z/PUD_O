#include "PackageUseDirHandler.h"
#include <iostream>

void debugShowPackages(const PackageUseDirHandler &packageUseDirHandler){
  auto pkgs = packageUseDirHandler.getPackagesHoler().getPackages();
  for (size_t i{}; i < pkgs.size(); ++i)
    std::cout << pkgs.at(i).getFullPackageInfoStr()
    << std::endl;
  std::cout << "Size is: " << pkgs.size()
  << std::endl;
}

int main() {
  try {
    PackageUseDirHandler packageUseDirHandler;
    debugShowPackages(packageUseDirHandler);
  }
  catch(std::exception &exc)
  {
    std::cout << exc.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
