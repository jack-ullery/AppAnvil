#include "command_caller.h"

#include <filesystem>
#include <fstream>
#include <glibmm/spawn.h>
#include <iostream>
#include <stdexcept>
#include <sys/stat.h>

CommandCaller::results CommandCaller::call_command(const std::vector<std::string> &command)
{
  results result;
  std::vector<std::string> envp = { "/usr/bin/", "/usr/sbin/", "/usr/local/bin" };
  Glib::spawn_sync(
    "/usr/sbin/", command, envp, Glib::SpawnFlags::SPAWN_SEARCH_PATH_FROM_ENVP, {}, &result.output, &result.error, &result.exit_status);
  return result;
}

std::string CommandCaller::call_command(const std::vector<std::string> &command, const std::string &return_on_error)
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

std::string CommandCaller::get_status(CommandCaller *caller)
{
  std::vector<std::string> command = { "pkexec", "aa-caller", "-s" };
  std::string return_on_error      = "{\"processes\": {}, \"profiles\": {}}";
  return caller->call_command(command, return_on_error);
}

std::string CommandCaller::get_unconfined(CommandCaller *caller)
{
  std::vector<std::string> command = { "pkexec", "aa-caller", "-u" };
  std::string return_on_error;
  return caller->call_command(command, return_on_error);
}

std::string CommandCaller::get_logs(CommandCaller *caller, const std::string &checkpoint_filepath)
{
  std::vector<std::string> command = { "pkexec", "aa-caller", "-l" };

  if (!checkpoint_filepath.empty()) {
    command.push_back(checkpoint_filepath);
  }

  std::string return_on_error;
  return caller->call_command(command, return_on_error);
}

std::string CommandCaller::load_profile(CommandCaller *caller, const std::string &fullFileName)
{
  std::vector<std::string> command = { "pkexec", "mv", fullFileName, "/etc/apparmor.d" };
  auto result                      = caller->call_command(command);

  if (result.exit_status != 0) {
    return result.error;
  }

  std::string fileName; // name without path
  fileName = fullFileName.substr(fullFileName.find_last_of('/') + 1);

  command = { "pkexec", "apparmor_parser", "-r", "/etc/apparmor.d/" + fileName };
  result  = caller->call_command(command);

  if (result.exit_status != 0) {
    return result.error;
  }

  return "Success: loading porfile" + fileName;
}

std::string CommandCaller::disable_profile(CommandCaller *caller, const std::string &profileName)
{
  std::vector<std::string> command = { "pkexec", "aa-disable", profileName };
  auto result                      = caller->call_command(command);

  if (result.exit_status != 0) {
    return result.error;
  }

  return "Success: disabling porfile" + profileName;
}

std::string CommandCaller::locate_profile(const std::string &profile, const std::initializer_list<std::string> &possible_profile_locations)
{
  for (const std::string &location : possible_profile_locations) {
    bool exists = std::filesystem::exists(location + profile);

    if (exists) {
      return location;
    }
  }

  return "";
}

std::string CommandCaller::execute_change(CommandCaller *caller,
                                          const std::string &profile,
                                          const std::string &old_status,
                                          const std::string &new_status)
{
  std::string status_command;

  if (new_status == old_status) {
    return "'" + profile + "' already set to " + new_status + ".";
  }

  if (new_status == "enforce") {
    status_command = "aa-enforce";
  } else if (new_status == "complain") {
    status_command = "aa-complain";
  } else if (new_status == "audit") {
    status_command = "aa-audit";
  } else if (new_status == "disabled") {
    status_command = "aa-disable";
  } else {
    return "Error: Illegal arguments passed to CommandCaller::execute_change.";
  }

  // Attempt to locate the profile in possible locations
  std::string profile_location = locate_profile(profile);

  // command to change the profile to the provided status
  std::vector<std::string> command = { "pkexec", status_command, "-d", profile_location, profile };
  auto result                      = caller->call_command(command);

  if (result.exit_status != 0) {
    return " Error changing the status of '" + profile + "': " + result.error;
  }

  return " Changed '" + profile + "' from " + old_status + " to " + new_status;
}

// Static public methods
std::string CommandCaller::get_status()
{
  CommandCaller caller;
  return get_status(&caller);
}

std::string CommandCaller::get_unconfined()
{
  CommandCaller caller;
  return get_unconfined(&caller);
}

std::string CommandCaller::get_logs(const std::string &checkpoint_filepath)
{
  CommandCaller caller;
  return get_logs(&caller, checkpoint_filepath);
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

// TODO(multiple-locations) handle different abstractions in multiple profile locations
std::vector<std::string> CommandCaller::get_abstractions(const std::string &path)
{
  std::vector<std::string> found = {};

  auto dir_iter = std::filesystem::directory_iterator(path);
  for (const auto &entry : dir_iter) {
    auto abstraction = entry.path().filename();
    found.push_back(abstraction);
  }

  return found;
}

std::map<std::string, CommandCaller::parser_profile_pair> CommandCaller::get_profiles(
  const std::initializer_list<std::string> &possible_profile_locations)
{
  std::map<std::string, std::pair<AppArmor::Parser, AppArmor::Profile>> found_profiles;

  // Iterate over every directory
  for (const auto &path : possible_profile_locations) {
    // Iterate over every file in a directory
    bool exists = std::filesystem::exists(path);
    if (exists) {
      auto files = std::filesystem::directory_iterator(path);
      for (const auto &entry : files) {
        // Attempt to parse file in directory
        try {
          if (entry.is_regular_file()) {
            AppArmor::Parser parsed(entry.path());

            // Add each profile found in the file
            auto profile_list = parsed.getProfileList();
            for (auto &profile : profile_list) {
              parser_profile_pair tuple{ parsed, profile };
              found_profiles.insert_or_assign(profile.name(), tuple);
            }
          }
        } catch (const std::exception &ex) {
          std::cerr << "Error reading file: " << entry.path() << std::endl;
          std::cerr << "\tMessage: " << ex.what() << std::endl;
        }
      }
    }
  }

  return found_profiles;
}