#include "command_caller.h"

#include <glibmm/spawn.h>
#include <iostream>
#include <stdexcept>

CommandCaller::results CommandCaller::call_command(const std::vector<std::string> &command)
{
  results result;
  Glib::spawn_sync("/usr/sbin/", command, Glib::SpawnFlags::SPAWN_DEFAULT, {}, &result.output, &result.error, &result.exit_status);
  return result;
}

std::string CommandCaller::call_command(const std::vector<std::string> &command, const std::string &return_on_error)
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

std::string CommandCaller::get_status_str(CommandCaller *caller)
{
  std::vector<std::string> command = {"pkexec", "aa-status", "--json"};
  std::string return_on_error      = "{\"processes\": {}, \"profiles\": {}";
  return caller->call_command(command, return_on_error);
}

std::string CommandCaller::get_unconfined(CommandCaller *caller)
{
  std::vector<std::string> command = {"pkexec", "aa-unconfined"};
  std::string return_on_error;
  return caller->call_command(command, return_on_error);
}

std::string CommandCaller::get_logs_str(CommandCaller *caller)
{
  std::vector<std::string> command = {"dmesg", "--ctime"};
  std::string return_on_error;
  return caller->call_command(command, return_on_error);
}

std::string CommandCaller::load_profile(CommandCaller *caller, const std::string &fullFileName)
{
  std::vector<std::string> command = {"pkexec", "mv", fullFileName, "/etc/apparmor.d"};
  auto result                      = caller->call_command(command);

  if(result.exit_status != 0) {
    return result.error;
  }

  std::string fileName; //name without path
  fileName = fullFileName.substr(fullFileName.find_last_of('/') + 1);

  command = {"pkexec", "apparmor_parser", "-r", "/etc/apparmor.d/" + fileName};
  result  = caller->call_command(command);

  if(result.exit_status != 0) {
    return result.error;
  }

  return "Success: loading porfile" + fileName;
}

std::string CommandCaller::disable_profile(CommandCaller *caller, const std::string &profileName)
{
  std::vector<std::string> command = {"pkexec", "aa-disable", profileName};
  auto result                      = caller->call_command(command);

  if(result.exit_status != 0) {
    return result.error;
  }

  return "Success: disabling porfile" + profileName;
}

std::string CommandCaller::execute_change(CommandCaller *caller, const std::string &profile, const std::string &old_status,
                                          const std::string &new_status)
{
  std::string status_command;

  if(new_status == "enforce" && old_status != "enforce") {
    status_command = "aa-enforce";
  } else if(new_status == "complain" && old_status != "complain") {
    status_command = "aa-complain";
  } else if(new_status == "disable" && old_status != "disabled") {
    status_command = "aa-disable";
  } else if(new_status == old_status) {
    return "'" + profile + "' already set to " + new_status + ".";
  } else {
    return "Error: Illegal arguments passed to CommandCaller::execute_change.";
  }

  // command to change the profile to the provided status
  std::vector<std::string> command = {"pkexec", status_command, profile};
  auto result                      = caller->call_command(command);

  if(result.exit_status != 0) {
    return " Error changing the status of '" + profile + "': " + result.error;
  }

  return " Changed '" + profile + "' from " + old_status + " to " + new_status;
}

// Static public methods
std::string CommandCaller::get_status_str()
{
  CommandCaller caller;
  return get_status_str(&caller);
}

std::string CommandCaller::get_unconfined()
{
  CommandCaller caller;
  return get_unconfined(&caller);
}

std::string CommandCaller::get_logs_str()
{
  CommandCaller caller;
  return get_logs_str(&caller);
}

std::string CommandCaller::load_profile(const std::string &fullFileName)
{
  CommandCaller caller;
  return load_profile(&caller, fullFileName);
}

std::string CommandCaller::disable_profile(const std::string &profileName)
{
  CommandCaller caller;
  return disable_profile(&caller, profileName);
}

std::string CommandCaller::execute_change(const std::string &profile, const std::string &old_status, const std::string &new_status)
{
  CommandCaller caller;
  return execute_change(&caller, profile, old_status, new_status);
}