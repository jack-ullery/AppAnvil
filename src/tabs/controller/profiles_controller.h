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

  virtual void add_data_to_record(const std::string &data);
  void refresh();

protected:
  bool on_button_event(GdkEventButton *event);
  bool on_key_event(GdkEventKey *event);
  void handle_profile_selected();

private:
  std::shared_ptr<ProfilesTab> prof;
  Adapter adapter;
};

#endif // TABS_CONTROLLER_PROFILES_CONTROLLER_H
