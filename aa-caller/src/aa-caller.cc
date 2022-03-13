#include "aa-caller.h"

#include <glibmm/spawn.h>
#include <iostream>

AppArmorCaller::results AppArmorCaller::call_command(const std::vector<std::string>& command)
{
  results result;
  Glib::spawn_sync("/usr/sbin/", command, Glib::SpawnFlags::SPAWN_DEFAULT, {}, &result.output, &result.error, &result.exit_status);
  return result;
}

std::string AppArmorCaller::call_command(const std::vector<std::string>& command, const std::string& return_on_error)
{
  if(command.empty()) {
    throw std::invalid_argument("'command' argument must be nonempty.");
  }

  results result = call_command(command);

  if(result.exit_status != 0) {
    std::cout << "Error calling '" << command[0] << "'. " << result.error << std::endl;
    return return_on_error;
  }

  return result.output;
}

std::string AppArmorCaller::get_status(AppArmorCaller *caller)
{
  std::vector<std::string> command = {"aa-status", "--json"};
  std::string return_on_error = "{\"processes\": {}, \"profiles\": {}}";
  return caller->call_command(command, return_on_error);
}

std::string AppArmorCaller::get_unconfined(AppArmorCaller *caller)
{
  std::vector<std::string> command = {"ps", "-A", "--format", "pid,ppid,user,context,comm", "--no-header"};
  std::string return_on_error = "";
  return caller->call_command(command, return_on_error);
}

std::string AppArmorCaller::get_logs(AppArmorCaller *caller)
{
  //2004-10-20 00:00:00 marks the release date of ubuntu, no earlier logs should exist
  std::vector<std::string> command = {"journalctl", "-b", "_AUDIT_TYPE=1400", "--since  \"2004-10-20 00:00:00\"","--output=json"};
  std::string return_on_error = "";
  return caller->call_command(command, return_on_error);
}

// Static public methods
std::string AppArmorCaller::get_status()
{
  AppArmorCaller caller;
  return get_status(&caller);
}

std::string AppArmorCaller::get_unconfined()
{
  AppArmorCaller caller;
  return get_unconfined(&caller);
}

std::string AppArmorCaller::get_logs()
{
  AppArmorCaller caller;
  return get_logs(&caller);
}

void print_usage(){
  std::cout << "aa-caller: A utility to do common AppArmor tasks (used by the AppAnvil Project)" << std::endl;
  std::cout << "   -s :  call \"aa-status\", to get the status of currently confined apps" << std::endl;
  std::cout << "   -u :  call \"aa-unconfined\", to get the status of some unconfined apps" << std::endl;
  std::cout << "   -l :  call \"dmesg\", to get all system logs" << std::endl;
}

int main(int argc, char **argv) {
  if(argc == 2){
    std::string arg(argv[1]);
    if(arg == "-s"){
      // Argument for: "get_status"
      std::cout << AppArmorCaller::get_status();
      return 0;
    }
    else if(arg == "-u"){
      // Argument for: "get_unconfined"
      std::cout << AppArmorCaller::get_unconfined();
      return 0;
    }
    else if(arg == "-l"){
      // Argument for: "get_logs"
      std::cout << AppArmorCaller::get_logs();
      return 0;
    }
  }

  print_usage();
  return 1;
}
