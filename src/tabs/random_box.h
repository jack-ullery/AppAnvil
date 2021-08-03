#ifndef GTKMM_EXAMPLE_RANDOMBOX_H
#define GTKMM_EXAMPLE_RANDOMBOX_H

#include <gtkmm/applicationwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/label.h>

class RandomBox : public Gtk::Box
{

public:
  RandomBox();
  virtual ~RandomBox() override;

protected:
  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  //Member widgets:
  Gtk::Box *r_box; 
  Gtk::Label *r_label;
};

#endif // GTKMM_EXAMPLE_RANDOMBOX_H
