#ifndef TABS_CONTROLLER_PROFILESCONTROLLER_H
#define TABS_CONTROLLER_PROFILESCONTROLLER_H

#include "status_controller.h"

#include <memory>
#include <string>
#include <vector>

template<class ProfilesTab, class ColumnRecord> class ProfilesController : public StatusController<ProfilesTab>
{
public:
  ProfilesController();
  virtual void add_data_to_record(const std::string &data);

private:
  const std::vector<ColumnHeader> col_names{ColumnHeader("Profile"), ColumnHeader("Status")};
  std::shared_ptr<ColumnRecord> col_record;
};

#endif // TABS_CONTROLLER_PROFILESCONTROLLER_H
