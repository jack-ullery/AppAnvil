#ifndef TABS_CONTROLLER_PROFILES_CONTROLLER_H
#define TABS_CONTROLLER_PROFILES_CONTROLLER_H

#include "../entries.h"
#include "status_controller.h"

#include <memory>
#include <string>
#include <vector>

template<class ProfilesTab, class Database, class Adapter>
class ProfilesController : public StatusController<ProfilesTab>
{
public:
  explicit ProfilesController(std::shared_ptr<Database> database);

  // Signal handler to handle when the user wants to change the status of a profile
  // This calls the default_change_fun with the correct values for the profile, old_status, and new_status
  void change_status(const std::string &path);

  // Sets the function to be used when changing the status of a profile, this is used in main_window.cc
  void set_status_change_signal_handler(sigc::slot<void(std::string, std::string, std::string)> change_fun);

  virtual void add_data_to_record(const std::string &data);
  void refresh();

protected:
  bool on_button_event(GdkEventButton *event);
  bool on_key_event(GdkEventKey *event);
  void handle_profile_selected();

private:
  std::shared_ptr<ProfilesTab> prof;
  Adapter adapter;
  sigc::slot<void(std::string, std::string, std::string)> profile_status_change_fun;
};

#endif // TABS_CONTROLLER_PROFILES_CONTROLLER_H
