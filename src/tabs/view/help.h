#ifndef TABS_VIEW_HELP_H
#define TABS_VIEW_HELP_H

#include "status.h"

#include <gtkmm/builder.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/enums.h>
#include <gtkmm/fixed.h>
#include <gtkmm/grid.h>
#include <gtkmm/liststore.h>
#include <gtkmm/notebook.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/textview.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treeviewcolumn.h>
#include <gtkmm/viewport.h>
#include <memory>
#include <string>
#include <vector>

class Help : public Gtk::ScrolledWindow
{
public:
  Help();

  void hide_searchbar();
  void show_searchbar(const bool &should_focus);
  void set_return_signal_handler(const Glib::SignalProxy<void>::SlotType &func);

protected:
  // Signal handlers
  void set_search_signal_handler(const Glib::SignalProxyProperty::SlotType &func);
  void on_search_changed();

private:
  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  // Widgets on the tab
  std::unique_ptr<Gtk::Box> h_box;
  std::unique_ptr<Gtk::Label> h_label;
  std::unique_ptr<Gtk::Box> h_searchbox;
  std::unique_ptr<Gtk::SearchEntry> h_search;
  std::unique_ptr<Gtk::Button> h_return_button;

  // The text that is shown
  // const std::string description;
  const std::string description;

  template<typename T_Widget> static std::unique_ptr<T_Widget> get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder);
};

#endif // TABS_VIEW_HELP_H
