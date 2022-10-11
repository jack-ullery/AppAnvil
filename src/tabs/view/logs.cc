#include "logs.h"
#include "info_box.h"

// TODO: Make get_widget common function somewhere
template<typename T_Widget>
std::unique_ptr<T_Widget> Logs::get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder)
{
  T_Widget *raw_addr = nullptr;
  builder->get_widget<T_Widget>(name, raw_addr);
  return std::unique_ptr<T_Widget>(raw_addr);
}

void Logs::set_information(std::list<std::pair<std::string, std::string>> data)
{
  uint index = 0;
  for (auto pair : data) {
    // If needed, create a new InfoBox to show this pair
    if (index >= info_vec.size()) {
      info_vec.push_back(InfoBox());
      l_log_info->add(info_vec[index]);
    }

    // Set the header and text of the next Info Box using data from the pair
    info_vec[index].set_text(pair.first, pair.second);
    info_vec[index].set_visible(true);

    index++;
  }

  // Set the uninitialized info boxes to be invisible
  while (index < info_vec.size()) {
    info_vec[index].set_visible(false);
    index++;
  }
}

Logs::Logs()
  : Status("/resources/log.glade"),
    builder{ Status::get_builder() },
    l_log_info{ Logs::get_widget<Gtk::Box>("l_log_info", builder) },
    info_vec()
{
  this->show_all();
}
