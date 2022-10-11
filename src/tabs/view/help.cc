#include "help.h"

#include <string>
#include <vector>

template<typename T_Widget>
std::unique_ptr<T_Widget> Help::get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder)
{
  T_Widget *raw_addr = nullptr;
  builder->get_widget<T_Widget>(name, raw_addr);
  return std::unique_ptr<T_Widget>(raw_addr);
}

Help::Help()
  : builder{ Gtk::Builder::create_from_resource("/resources/help.glade") },
    h_box{ Help::get_widget<Gtk::Box>("h_box", builder) },
    h_label{ Help::get_widget<Gtk::Label>("h_label", builder) },
    h_searchbox{ Help::get_widget<Gtk::Box>("h_searchbox", builder) },
    h_search{ Help::get_widget<Gtk::SearchEntry>("h_search", builder) },
    h_return_button{ Help::get_widget<Gtk::Button>("h_return_button", builder) },
    description{ h_label->get_label() }
{
  auto search_func = sigc::mem_fun(*this, &Help::on_search_changed);
  set_search_signal_handler(search_func);

  h_label->set_use_markup(true);
  h_box->set_hexpand();
  h_box->set_vexpand();

  // Make the searchbox invisible, until show_searchbar() is called
  h_searchbox->set_no_show_all(true);
  hide_searchbar();

  this->add(*h_box);
}

void Help::hide_searchbar()
{
  h_searchbox->hide();
}

void Help::show_searchbar(const bool &should_focus)
{
  h_searchbox->show();

  if (should_focus) {
    h_search->grab_focus();
  }
}

void Help::set_return_signal_handler(const Glib::SignalProxy<void>::SlotType &func)
{
  h_return_button->signal_clicked().connect(func, true);
}

void Help::set_search_signal_handler(const Glib::SignalProxyProperty::SlotType &func)
{
  h_search->signal_search_changed().connect(func, true);
}

// TODO(apparmor): Need to make searchbar more like the one used in status.cc
void Help::on_search_changed()
{
  std::string search = h_search->get_text();

  if (search.empty() || search == "b" || search == "i" || search == "u") {
    h_label->set_text(description);
    h_label->set_use_markup(true);
    return;
  }

  size_t pos = description.find(search);

  std::string description_copy(description);
  while (pos != std::string::npos) {
    auto replacement_string = "<span bgcolor=\"yellow\">" + description_copy.substr(pos, search.size()) + "</span>";
    description_copy.replace(pos, search.size(), replacement_string);
    pos = description_copy.find(search, pos + replacement_string.size());
  }

  h_label->set_text(description_copy);
  h_label->set_use_markup(true);
}
