#include "aa-caller.h"

#include <cstdlib>
#include <glibmm/spawn.h>
#include <iostream>

AppArmorCaller::results AppArmorCaller::call_command(const std::vector<std::string> &command)
{
  results result;
  std::vector<std::string> envp = {"/usr/bin/", "/usr/sbin/"};
  Glib::spawn_sync("/usr/sbin/",
                   command,
                   envp,
                   Glib::SpawnFlags::SPAWN_SEARCH_PATH_FROM_ENVP,
                   {},
                   &result.output,
                   &result.error,
                   &result.exit_status);
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
    if(command[0] != "ausearch" || result.exit_status != 256) {
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
  std::vector<std::string> command = {"ausearch", "--raw", "-m", "AVC", "--checkpoint", filename};

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
