#ifndef TABS_VIEW_LOGS_H
#define TABS_VIEW_LOGS_H

#include "info_box.h"
#include "jsoncpp/json/json.h"
#include "status.h"

#include <memory>

class Logs : public Status
{
public:
  Logs();

  enum InformationType
  {
    LOG_STATUS
  };

  void set_information(const std::list<std::pair<std::string, std::string>> &data);

private:
  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  // Information pane
  std::unique_ptr<Gtk::Box> l_log_info;

  // Widgets
  std::vector<InfoBox> info_vec;
};

#endif // TABS_VIEW_LOGS_H