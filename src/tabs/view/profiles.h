#ifndef TABS_PROFILES_H
#define TABS_PROFILES_H

#include "status.h"

#include <memory>
#include <string>
#include <vector>

class Profiles : public Status
{
public:
  explicit Profiles();

  // Sets the function to be used when changing the status of a profile, this is used in main_window.cc
  void set_status_change_signal_handler(sigc::slot<void(std::string, std::string, std::string)> change_fun);

protected:
  // Signal handler to handle when the user wants to change the status of a profile
  // This calls the default_change_fun with the correct values for the profile, old_status, and new_status
  void change_status();

private:
  void default_change_fun(const std::string &a, const std::string &b, const std::string &c);
  sigc::slot<void(std::string, std::string, std::string)> profile_status_change_fun;
};

#endif // TABS_PROFILES_H
