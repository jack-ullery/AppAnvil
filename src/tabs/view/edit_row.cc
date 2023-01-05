#include "common.h"
#include "edit_row.h"
#include <gtkmm/enums.h>

EditRow::EditRow(const std::string &title)
  : builder{ Gtk::Builder::create_from_resource("/resources/edit_row.glade") },
    edit_row{ Common::get_widget<Gtk::Box>("edit_row", builder)},
    edit_title{ Common::get_widget<Gtk::Label>("edit_title", builder)}
{
  edit_title->set_label(title);

  this->set_hexpand(true);
  this->add(*edit_row);
}