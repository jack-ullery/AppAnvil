#include "command_caller.h"

#include <glibmm/spawn.h>
#include <iostream>
#include <stdexcept>

CommandCaller::results CommandCaller::call_command(std::vector<std::string> command){
  results result;
  Glib::spawn_sync("/usr/sbin/", command, Glib::SpawnFlags::SPAWN_DEFAULT, {}, &result.output, &result.error, &result.exit_status);
  return result;
}

std::string CommandCaller::call_command(std::vector<std::string> command, std::string return_on_error){
  if(command.empty()){
    throw std::invalid_argument("'command' argument must be nonempty.");
  }

  results result = call_command(command);

  if(result.exit_status != 0){
    std::cout << "Error calling '"<< command[0] <<"'. " << result.error << std::endl;
    return return_on_error;
  }

  return result.output;
}

std::string CommandCaller::get_status_str(){
  std::vector<std::string> command = {"pkexec", "aa-status","--json"};
  std::string return_on_error = "{\"processes\": {}, \"profiles\": {}";
  return call_command(command, return_on_error);
}

std::string CommandCaller::get_unconfined(){
  std::vector<std::string> command = {"pkexec", "aa-unconfined"};
  std::string return_on_error = "";
  return call_command(command, return_on_error);
}

std::string CommandCaller::get_logs_str(){
  std::vector<std::string> command = {"dmesg"};
  std::string return_on_error = "";
  return call_command(command, return_on_error);
}

std::string CommandCaller::load_profile(std::string fullFileName){
  std::vector<std::string> command = {"pkexec", "mv", fullFileName, "/etc/apparmor.d"};
  auto result = call_command(command);

  if(result.exit_status != 0){
    return result.error;
  }

  std::string fileName; //name without path
  fileName = fullFileName.substr(fullFileName.find_last_of("/") + 1);

  command = {"pkexec", "apparmor_parser", "-r", "/etc/apparmor.d/" + fileName};
  result = call_command(command);

  if(result.exit_status != 0){
    return result.error;
  }

  return "Success: loading porfile" + fileName;
}

std::string CommandCaller::disable_profile(std::string profileName){
  std::vector<std::string> command = {"pkexec", "aa-disable", profileName};
  auto result = call_command(command);

  if(result.exit_status != 0){
    return result.error;
  }

  return "Success: disabling porfile" + profileName;
}