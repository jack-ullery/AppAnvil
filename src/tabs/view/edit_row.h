#ifndef TABS_EDIT_ROW_H
#define TABS_EDIT_ROW_H

#include <gtkmm/bin.h>
#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/label.h>

class EditRow : public Gtk::Bin
{
public:
  explicit EditRow(const std::string &title);

private:
  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  // Widgets
  std::unique_ptr<Gtk::Box> edit_row;
  std::unique_ptr<Gtk::Label> edit_title;
};

#endif