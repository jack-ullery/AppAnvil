#include "status.h"
#include "common.h"

#include "jsoncpp/json/json.h"

#include <iostream>
#include <regex>
#include <sstream>
#include <string>

void Status::set_status_label_text(const std::string &str)
{
  s_found_label->set_text(str);
}

void Status::set_refresh_signal_handler(const Glib::SignalProxyProperty::SlotType &func)
{
  s_search->signal_search_changed().connect(func, true);
  s_use_regex->signal_clicked().connect(func, true);
  s_match_case->signal_clicked().connect(func, true);
  s_whole_word->signal_clicked().connect(func, true);
}

std::shared_ptr<Gtk::TreeView> Status::get_view()
{
  return s_view;
}

std::shared_ptr<Gtk::ScrolledWindow> Status::get_window()
{
  return s_win;
}

SearchInfo Status::get_search_info()
{
  SearchInfo info(s_search->get_text(), s_use_regex->get_active(), s_match_case->get_active(), s_whole_word->get_active());
  return info;
}

Glib::RefPtr<Gtk::Builder> Status::get_builder()
{
  return builder;
}

void Status::hide_searchbar()
{
  s_searchbox->hide();
}

void Status::show_searchbar(const bool &should_focus)
{
  s_searchbox->show();

  if (should_focus) {
    s_search->grab_focus();
  }
}

Status::Status(const std::string &glade_resource)
  : builder{ Gtk::Builder::create_from_resource(glade_resource) },
    s_view{ Common::get_widget_shared<Gtk::TreeView>("s_view", builder) },
    s_win{ Common::get_widget_shared<Gtk::ScrolledWindow>("s_win", builder) },
    s_box{ Common::get_widget<Gtk::Box>("s_box", builder) },
    s_searchbox{ Common::get_widget<Gtk::Box>("s_searchbox", builder) },
    s_search{ Common::get_widget<Gtk::SearchEntry>("s_search", builder) },
    s_use_regex{ Common::get_widget<Gtk::CheckButton>("s_use_regex", builder) },
    s_match_case{ Common::get_widget<Gtk::CheckButton>("s_match_case", builder) },
    s_whole_word{ Common::get_widget<Gtk::CheckButton>("s_whole_word", builder) },
    s_found_label{ Common::get_widget<Gtk::Label>("s_found_label", builder) }
{
  s_view->set_activate_on_single_click(true);

  s_win->set_shadow_type(Gtk::ShadowType::SHADOW_NONE);
  s_win->set_policy(Gtk::PolicyType::POLICY_AUTOMATIC, Gtk::PolicyType::POLICY_AUTOMATIC);
  s_win->set_hexpand();
  s_win->set_vexpand();

  // Make the searchbox invisible, until show_searchbar() is called
  s_searchbox->set_no_show_all(true);
  hide_searchbar();

  this->append(*s_box);
}

Status::Status()
  : Status("/resources/status.glade")
{
}
