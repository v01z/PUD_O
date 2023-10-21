#include "PackageUseDirHandler.h"
#include <iostream>

//-----------------------------------------------------------------

int main(int argc, char **argv) {

  std::cout << KMAG << "pudo: Package use dir organizer for Gentoo Linux"
            << KNRM << std::endl;
  std::cout << argc << std::endl; // debug

  if ((argc == 2) &&
      ((std::string(argv[1]) == "--help") || (std::string(argv[1]) == "-h"))) {
    std::cout << "Usage:\n"
              << KYEL << argv[0] << KNRM << std::endl
              << "or:\n"
              << KYEL << argv[0] << " --exclude \'files to exclude\'" << KNRM
              << std::endl;
    return EXIT_SUCCESS;
  }

  std::vector<std::filesystem::path> argvVec{};

  if (argc > 2 &&
      (std::string(argv[1]) == "--exclude" || std::string(argv[1]) == "-e")) {
    for (size_t i{2}; i < argc; ++i) {
      argvVec.push_back(argv[i]);
    }
  }

  try {
    PackageUseDirHandler packageUseDirHandler{std::move(argvVec)};

    std::cout << "\nAll the work has been done. Have a nice day!" << std::endl;
  } catch (std::exception &exc) {
    std::cout << KRED << "\nError:\n"
              << KYEL << exc.what() << KNRM << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

//-----------------------------------------------------------------
