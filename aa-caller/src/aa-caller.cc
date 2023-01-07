#include "aa-caller.h"

#include <glibmm/spawn.h>
#include <iostream>

AppArmorCaller::results AppArmorCaller::call_command(const std::vector<std::string> &command)
{
  results result;
  Glib::spawn_sync("/usr/sbin/", command, Glib::SpawnFlags::SEARCH_PATH, {}, &result.output, &result.error, &result.exit_status);
  return result;
}

std::string AppArmorCaller::call_command(const std::vector<std::string> &command, const std::string &return_on_error)
{
  if (command.empty()) {
    throw std::invalid_argument("'command' argument must be nonempty.");
  }

  results result = call_command(command);

  if (result.exit_status != 0) {
    std::cout << "Error calling '" << command[0] << "'. " << result.error << std::endl;
    return return_on_error;
  }

  return result.output;
}

std::string AppArmorCaller::get_status(AppArmorCaller *caller)
{
  std::vector<std::string> command = { "aa-status", "--json" };
  std::string return_on_error      = "{\"processes\": {}, \"profiles\": {}}";
  return caller->call_command(command, return_on_error);
}

std::string AppArmorCaller::get_unconfined(AppArmorCaller *caller)
{
  std::vector<std::string> command = { "ps", "-A", "--format", "pid,ppid,user,context,comm", "--no-header" };
  std::string return_on_error;
  return caller->call_command(command, return_on_error);
}

std::string AppArmorCaller::get_logs(AppArmorCaller *caller)
{
  std::vector<std::string> command = { "journalctl", "-r", "_AUDIT_TYPE=1400", "--output=json", "--show-cursor" };
  std::string return_on_error;
  return caller->call_command(command, return_on_error);
}

std::string AppArmorCaller::get_logs(AppArmorCaller *caller, const std::string &cursor)
{
  std::vector<std::string> command = { "journalctl", "-r", "_AUDIT_TYPE=1400", "--output=json", "--show-cursor", "--after-cursor", cursor };
  std::string return_on_error;
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

std::string AppArmorCaller::get_logs(const std::string &cursor)
{
  AppArmorCaller caller;
  return get_logs(&caller, cursor);
}
