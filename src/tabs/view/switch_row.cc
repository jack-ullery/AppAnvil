#include "common.h"
#include "switch_row.h"

SwitchRow::SwitchRow(const std::string &title, const std::string &subtitle)
  : builder{ Gtk::Builder::create_from_resource("/resources/switcher_row.glade") },
    switch_row{ Common::get_widget<Gtk::Box>("switch_row", builder)},
    switch_title{ Common::get_widget<Gtk::Label>("switch_title", builder)},
    switch_subtitle{ Common::get_widget<Gtk::Label>("switch_subtitle", builder)},
    switch_toggle{ Common::get_widget<Gtk::Switch>("switch_toggle", builder)}
{
  switch_title->set_text(title);
  switch_subtitle->set_text(subtitle);

  this->add(*switch_row);
}
