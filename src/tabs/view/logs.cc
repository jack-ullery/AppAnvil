#include "logs.h"

// TODO: Make get_widget common function somewhere
template<typename T_Widget>
std::unique_ptr<T_Widget> Logs::get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder)
{
  T_Widget *raw_addr = nullptr;
  builder->get_widget<T_Widget>(name, raw_addr);
  return std::unique_ptr<T_Widget>(raw_addr);
}

void Logs::set_information(InformationType type, std::string text)
{
  switch(type) {
    case LOG_STATUS:
      status_info.set_text(text);
      break;
  }
}

Logs::Logs()
  : Status("/resources/log.glade"),
    builder{ Status::get_builder() },
    l_log_info{ Logs::get_widget<Gtk::Box>("l_log_info", builder) },
    status_info("Status")
{
  l_log_info->add(status_info);
  this->show_all();
}
