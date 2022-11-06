#include "switch_box.h"
#include <gtkmm/enums.h>
#include <string>

SwitchBox::SwitchBox(const std::vector<string_tuple> &tuples)
{
  for(auto tuple : tuples) {
    const std::string &title    = std::get<0>(tuple);
    const std::string &subtitle = std::get<1>(tuple);
    // const std::string &id       = std::get<2>(tuple);

    auto ptr = std::shared_ptr<SwitchRow>(new SwitchRow(title, subtitle));
    switch_list.push_back(ptr);
    this->add(*ptr);
  }

  // Tell this box to not expand
  this->set_hexpand(false);
  this->set_vexpand(false);
  this->set_halign(Gtk::ALIGN_START);
  this->set_valign(Gtk::ALIGN_START);
}
