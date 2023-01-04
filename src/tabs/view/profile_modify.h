#ifndef TABS_PROFILE_MODIFY_H
#define TABS_PROFILE_MODIFY_H

#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/scrolledwindow.h>
#include <list>
#include <memory>
#include <string>
#include <tuple>

class ProfileModify : public Gtk::ScrolledWindow
{
public:
  explicit ProfileModify(const std::string &profile_name);

private:
  // VBox which holds all the widgets
  std::unique_ptr<Gtk::Box> m_box;

  // Widgets
  Gtk::Label header;
};

#endif
