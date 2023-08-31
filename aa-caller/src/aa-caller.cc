#include "aa-caller.h"

#include <cstdlib>
#include <filesystem>
#include <glibmm/spawn.h>
#include <iostream>

AppArmorCaller::results AppArmorCaller::call_command(const std::vector<std::string> &command)
{
  results result;
  std::vector<std::string> possible_directories = { "/usr/bin/", "/usr/sbin/" };

  // Search for the directory where the binary is located
  // Note: Normally we would use the flag Glib::SpawnFlags::SPAWN_SEARCH_PATH_FROM_ENVP
  //       to make Glib search for this. However, I was getting an uncatchable Glib::SpawnError
  //       that would crash aa-caller. There probably is a better solution, but I resorted to
  //       manually searching for the binary to avoid this issue.
  std::string command_dir = "";
  for (const auto &dir : possible_directories) {
    std::filesystem::path path(dir);
    path.append(command[0]);

    if (std::filesystem::exists(path)) {
      command_dir = dir;
      break;
    }
  }

  if (!command_dir.empty()) {
    // We found the binary's location, so lets run the command
    std::vector<std::string> envp{};
    Glib::spawn_sync(command_dir, command, envp, Glib::SpawnFlags::SPAWN_DEFAULT, {}, &result.output, &result.error, &result.exit_status);
  } else {
    // If we could not find the binary, then return an error
    std::stringstream error_message;
    error_message << command[0] << ": Command not found" << std::endl;

    result.exit_status = 127;
    result.error       = error_message.str();
  }
  return result;
}

std::string AppArmorCaller::call_command(const std::vector<std::string> &command, const std::string &return_on_error)
{
  if (command.empty()) {
    throw std::invalid_argument("'command' argument must be nonempty.");
  }

  results result = call_command(command);

  // If there was an error, print it out to std::cerr
  if (result.exit_status != 0) {
    // ausearch returns and error code if no data was found
    // I do not consider that an issue, so I suppress it here
    if (command[0] != "ausearch" || result.exit_status != 256) {
      std::cerr << "Error calling '" << command[0] << "' returned (" << result.exit_status << "). " << result.error << std::endl;
      return return_on_error;
    }
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

std::string AppArmorCaller::get_logs(AppArmorCaller *caller, const std::string &filename)
{
  std::vector<std::string> command = { "ausearch", "--raw", "-m", "AVC", "--checkpoint", filename };

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

  // Seed the random number generator using the time
  srand(time(NULL));

  // Create a random filename with the following format '/tmp/appanvil-$RAND'
  std::stringstream stream;
  stream << "/tmp/appanvil-" << rand();

  // Call get_logs and append the output to what we are interested in
  std::string output = get_logs(&caller, stream.str());
  stream << std::endl << output;

  return stream.str();
}

std::string AppArmorCaller::get_logs(const std::string &filename)
{
  AppArmorCaller caller;
  return get_logs(&caller, filename);
}
