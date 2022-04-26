#include "status.h"

#include "jsoncpp/json/json.h"

#include <iostream>
#include <regex>
#include <sstream>
#include <string>

template<typename T_Widget> std::unique_ptr<T_Widget> Status::get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder)
{
  T_Widget *raw_addr = nullptr;
  builder->get_widget<T_Widget>(name, raw_addr);
  return std::unique_ptr<T_Widget>(raw_addr);
}

void Status::set_status_label_text(const std::string &str) { s_found_label->set_text(str); }

void Status::set_apply_label_text(const std::string &str) { s_apply_info_text->set_text(str); }

void Status::set_refresh_signal_handler(const Glib::SignalProxyProperty::SlotType &func)
{
  s_search->signal_search_changed().connect(func, true);
  s_use_regex->signal_clicked().connect(func, true);
  s_match_case->signal_clicked().connect(func, true);
  s_whole_word->signal_clicked().connect(func, true);
}

// for handling the apply button being clicked when an appopriate choice has been made in the drop-down
void Status::set_apply_signal_handler(const Glib::SignalProxyProperty::SlotType &func)
{
  s_apply_button->signal_clicked().connect(func, true);
}

std::shared_ptr<Gtk::TreeView> Status::get_view() { return s_view; }

std::shared_ptr<Gtk::ScrolledWindow> Status::get_window() { return s_win; }

Glib::ustring Status::get_selection_text() const { return s_status_selection->get_active_text(); }

void Status::remove_status_selection()
{
  s_box->remove(*s_selection_box);
  s_selection_box->hide();
}

Status::Status()
    : builder{Gtk::Builder::create_from_resource("/resources/status.glade")}, s_view{Status::get_widget<Gtk::TreeView>("s_view", builder)},
      s_win{Status::get_widget<Gtk::ScrolledWindow>("s_win", builder)}, s_box{Status::get_widget<Gtk::Box>("s_box", builder)},
      s_search{Status::get_widget<Gtk::SearchEntry>("s_search", builder)}, s_use_regex{Status::get_widget<Gtk::CheckButton>("s_use_regex",
                                                                                                                            builder)},
      s_match_case{Status::get_widget<Gtk::CheckButton>("s_match_case", builder)}, s_whole_word{Status::get_widget<Gtk::CheckButton>(
                                                                                       "s_whole_word", builder)},
      s_found_label{Status::get_widget<Gtk::Label>("s_found_label", builder)}, s_selection_box{Status::get_widget<Gtk::Box>(
                                                                                   "s_selection_box", builder)},
      s_apply_button{Status::get_widget<Gtk::Button>("s_apply_button", builder)}, s_apply_info_text{Status::get_widget<Gtk::Label>(
                                                                                      "s_apply_info_text", builder)},
      s_status_selection{Status::get_widget<Gtk::ComboBoxText>("s_status_selection", builder)}
{
  s_win->set_shadow_type(Gtk::ShadowType::SHADOW_NONE);
  s_win->set_policy(Gtk::PolicyType::POLICY_AUTOMATIC, Gtk::PolicyType::POLICY_AUTOMATIC);
  s_win->set_hexpand();
  s_win->set_vexpand();

  this->add(*s_box);
}