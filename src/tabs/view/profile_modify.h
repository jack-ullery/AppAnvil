#ifndef TABS_PROFILE_MODIFY_H
#define TABS_PROFILE_MODIFY_H

#include "switch_box.h"
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/scrolledwindow.h>
#include <list>
#include <memory>

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
};

#endif
