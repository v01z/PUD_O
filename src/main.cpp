#include "PackageUseDirHandler.h"
#include <iostream>

//-----------------------------------------------------------------

int main(int argc, char **argv) {

  std::cout << KMAG << "pudo: Package use dir organizer for Gentoo Linux"
      << KNRM << std::endl;

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
  }
  catch(std::exception &exc)
  {
    std::cout << exc.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

//-----------------------------------------------------------------
