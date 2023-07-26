#ifndef TABS_VIEW_HELP_H
#define TABS_VIEW_HELP_H

#include <gtkmm/builder.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/popover.h>
#include <memory>
#include <string>
#include <vector>

class Help : public Gtk::ToggleButton
{
public:
  Help();

protected:
  void on_help_toggle();

private:
  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  // Widgets on the tab
  std::unique_ptr<Gtk::Popover> help_profile;
  std::unique_ptr<Gtk::Popover> help_profile_modify;
  std::unique_ptr<Gtk::Popover> help_logs;
};

#endif // TABS_VIEW_HELP_H
