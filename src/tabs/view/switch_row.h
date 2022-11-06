#ifndef TABS_SWITCH_ROW_H
#define TABS_SWITCH_ROW_H

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/label.h>
#include <gtkmm/switch.h>

class SwitchRow : public Gtk::VBox
{
public:
  SwitchRow(const std::string &title, const bool &active);

private:
  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  // Widgets
  std::unique_ptr<Gtk::Box> switch_row;
  std::unique_ptr<Gtk::Label> switch_title;
  std::unique_ptr<Gtk::Switch> switch_toggle;
};

#endif
