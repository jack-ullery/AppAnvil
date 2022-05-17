#include "about.h"

#include <string>
#include <vector>

template<typename T_Widget> std::unique_ptr<T_Widget> About::get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder)
{
  T_Widget *raw_addr = nullptr;
  builder->get_widget<T_Widget>(name, raw_addr);
  return std::unique_ptr<T_Widget>(raw_addr);
}

About::About()
    : builder{Gtk::Builder::create_from_file("./resources/about.glade")}, 
      a_box{About::get_widget<Gtk::Box>("a_box", builder)},
      a_label{About::get_widget<Gtk::Label>("a_label", builder)},
      a_searchbox{About::get_widget<Gtk::Box>("a_searchbox", builder)},
      a_search{About::get_widget<Gtk::SearchEntry>("a_search", builder)},
      description{a_label->get_label()}
{
  auto search_func = sigc::mem_fun(*this, &About::on_search_changed);
  set_search_signal_handler(search_func);

  a_label->set_use_markup(true);
  a_box->set_hexpand();
  a_box->set_vexpand();

  this->add(*a_box);
}

void About::hide_searchbar()
{
  a_searchbox->hide();
}

void About::show_searchbar()
{
  a_searchbox->show();
}

void About::set_search_signal_handler(const Glib::SignalProxyProperty::SlotType &func) {
  a_search->signal_search_changed().connect(func, true);
}

// TODO(apparmor): Need to make searchbar more like the one used in status.cc
void About::on_search_changed() 
{
  std::string search = a_search->get_text();
  
  if (search.empty() || search == "b" || search == "i" || search == "u") {
    a_label->set_text(description);
    a_label->set_use_markup(true);
    return;
  }

  size_t pos = description.find(search);
  
  std::string description_copy(description);
  while (pos != std::string::npos) {
    auto replacement_string = "<span bgcolor=\"yellow\">" + description_copy.substr(pos, search.size()) + "</span>";
    description_copy.replace(pos, search.size(), replacement_string);
    pos = description_copy.find(search, pos + replacement_string.size());
  }

  a_label->set_text(description_copy);
  a_label->set_use_markup(true);
}
