#ifndef TABS_VIEW_INFO_BOX_H
#define TABS_VIEW_INFO_BOX_H

#include <gtkmm/bin.h>
#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/label.h>
#include <memory>

class InfoBox : public Gtk::Bin
{
public:
  InfoBox();

  // Sets the header and main text shown by the InfoBox
  void set_text(const std::string &header, const std::string &text);

private:
  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  // Widgets
  std::unique_ptr<Gtk::Box> i_box;
  std::unique_ptr<Gtk::Label> i_header;
  std::unique_ptr<Gtk::Label> i_text;
};

#endif // TABS_VIEW_INFO_BOX_H