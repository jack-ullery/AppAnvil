#include "info_box.h"

template<typename T_Widget>
std::unique_ptr<T_Widget> InfoBox::get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder)
{
  T_Widget *raw_addr = nullptr;
  builder->get_widget<T_Widget>(name, raw_addr);
  return std::unique_ptr<T_Widget>(raw_addr);
}

void InfoBox::set_text(const std::string &header, const std::string &text)
{
  i_header->set_text(header);
  i_text->set_text(text);
}

InfoBox::InfoBox()
  : builder{ Gtk::Builder::create_from_resource("/resources/info_box.glade") },
    i_box{ InfoBox::get_widget<Gtk::Box>("i_box", builder) },
    i_header{ InfoBox::get_widget<Gtk::Label>("i_header", builder) },
    i_text{ InfoBox::get_widget<Gtk::Label>("i_text", builder) }
{
  i_header->set_text("");
  i_text->set_text("");
  this->add(*i_box);
}
