#include "aa-caller.h"

#include <iostream>

void print_usage()
{
  std::cout << "A utility to do common AppArmor tasks (used by the AppAnvil Project)" << std::endl << std::endl;
  std::cout << "Usage: aa-caller <option>" << std::endl << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "   -s :  call \"aa-status\", to get the status of currently confined apps" << std::endl;
  std::cout << "   -u :  call \"aa-unconfined\", to get the status of some unconfined apps" << std::endl;
  std::cout << "   -l [cursor]:  call \"journalctl\", to get all system logs, starting after an optional cursor" << std::endl;
}

int main(int argc, char **argv)
{
  if (argc == 2) {
    std::string arg(argv[1]);
    if (arg == "-s") {
      // Argument for: "get_status"
      std::cout << AppArmorCaller::get_status();
      return 0;
    } else if (arg == "-u") {
      // Argument for: "get_unconfined"
      std::cout << AppArmorCaller::get_unconfined();
      return 0;
    } else if (arg == "-l") {
      // Argument for: "get_logs"
      std::cout << AppArmorCaller::get_logs();
      return 0;
    }
  } else if (argc == 3) {
    std::string arg(argv[1]);
    std::string data(argv[2]);

    if (arg == "-l") {
      // Argument for: "get_logs"
      std::cout << AppArmorCaller::get_logs(data);
      return 0;
    }
  }

  print_usage();
  return 1;
}
