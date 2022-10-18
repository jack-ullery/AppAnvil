#include "common.h"
#include "info_box.h"

void InfoBox::set_text(const std::string &header, const std::string &text)
{
  i_header->set_text(header);
  i_text->set_text(text);
}

InfoBox::InfoBox()
  : builder{ Gtk::Builder::create_from_resource("/resources/info_box.glade") },
    i_box{ Common::get_widget<Gtk::Box>("i_box", builder) },
    i_header{ Common::get_widget<Gtk::Label>("i_header", builder) },
    i_text{ Common::get_widget<Gtk::Label>("i_text", builder) }
{
  i_header->set_text("");
  i_text->set_text("");
  this->add(*i_box);
}
