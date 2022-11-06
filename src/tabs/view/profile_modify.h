#ifndef TABS_PROFILE_MODIFY_H
#define TABS_PROFILE_MODIFY_H

#include "switch_box.h"
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/scrolledwindow.h>
#include <list>
#include <memory>
#include <tuple>

class ProfileModify : public Gtk::ScrolledWindow
{
public:
  ProfileModify();

private:
  // VBox which holds all the widgets
  std::unique_ptr<Gtk::Box> m_box;

  // Widgets
  Gtk::Label header;
  std::list<std::shared_ptr<SwitchBox>> switch_box_list;

  // Rows for each box (title, subtitle, id)
  std::vector<string_tuple> rows {
    string_tuple("Example Title", "First",  "Example Id"),
    string_tuple("Example Title", "Second", "Example Id")
  };
};

#endif
