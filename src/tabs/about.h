#ifndef TABS_ABOUT_H
#define TABS_ABOUT_H

#include "status.h"

#include <gtkmm/builder.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/enums.h>
#include <gtkmm/filechooser.h>
#include <gtkmm/filechooserdialog.h>
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
#include <memory>
#include <string>
#include <vector>

class About : public Gtk::ScrolledWindow
{
public:
  About();

protected:
  // Signal handler
  void on_fc_button_clicked();
  void handle_file_chooser_dialogue(int response_id);
  void on_confirm_clicked();

private:
  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  // Widgets on the tab
  std::unique_ptr<Gtk::Box> a_box;
  //std::unique_ptr<Gtk::Fixed> a_fixed;
  std::unique_ptr<Gtk::Label> a_label;

  template<typename T_Widget> static std::unique_ptr<T_Widget> get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder);
};

#endif // TABS_ABOUT_H
