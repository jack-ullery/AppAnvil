#include "about.h"

#include "../threads/command_caller.h"

#include <iostream>
#include <string>
#include <vector>

template<typename T_Widget> std::unique_ptr<T_Widget> About::get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder)
{
  T_Widget *raw_addr = nullptr;
  builder->get_widget<T_Widget>(name, raw_addr);
  return std::unique_ptr<T_Widget>(raw_addr);
}

About::About()
    : builder{Gtk::Builder::create_from_file("./resources/about.glade")}, a_box{About::get_widget<Gtk::Box>("a_box", builder)},
    a_win{About::get_widget<Gtk::ScrolledWindow>("a_win", builder)}, a_view{About::get_widget<Gtk::Viewport>("a_view", builder)},
    a_label{About::get_widget<Gtk::Label>("a_label", builder)}, a_box_hz{About::get_widget<Gtk::Box>("a_box_hz", builder)},
    link_btn{About::get_widget<Gtk::Button>("link_btn", builder)}, a_search{About::get_widget<Gtk::SearchEntry>("a_search", builder)}

{
  auto button_func = sigc::mem_fun(*this, &About::on_confirm_clicked);
  set_link_btn_signal_handler(button_func);
  auto search_func = sigc::mem_fun(*this, &About::on_search_changed);
  set_search_signal_handler(search_func);
  a_label->set_text(get_marked_up_text());
  a_label->set_use_markup(true);
  a_box->set_hexpand();
  a_box->set_vexpand();

  this->add(*a_box);
}


void About::set_link_btn_signal_handler(const Glib::SignalProxyProperty::SlotType &func){
  link_btn->signal_clicked().connect(func, true);
}


void About::on_confirm_clicked(){
  system("/usr/bin/firefox https://gitlab.com/apparmor/apparmor/-/wikis/Documentation");
  return;
}


void About::set_search_signal_handler(const Glib::SignalProxyProperty::SlotType &func) {
  a_search->signal_search_changed().connect(func, true);
}

void About::on_search_changed() 
{
  std::string label = get_marked_up_text();
  std::string search = a_search->get_text();

  if (search.empty() || search == "b" || search == "i" || search == "u") {
    a_label->set_text(label);
    a_label->set_use_markup(true);
    return;
  }
  
  size_t pos = label.find(search);
  std::string replacement_string = "<span bgcolor=\"yellow\">" + search + "</span>";

  while (pos != std::string::npos) {
    label.replace(pos, search.size(), replacement_string);
    pos = label.find(search, pos + replacement_string.size());
  }

  a_label->set_text(label);
  a_label->set_use_markup(true);
}


std::string About::get_marked_up_text() {
    return "<b>What is AppAnvil?</b>\n"
        "AppAnvil is a graphical user interface (GUI) designed to facilitate the use of AppArmor.\n\n"
        "<b>What is AppArmor?</b>\n"
        "AppArmor is a Linux security module that protects your system from potential threats by imposing Mandatory Access Control (MAC).\n"
        "It allows the user to secure individual applications by defining and restricting what resources an application can access or share.\n\n"
        "<b>What is a profile?</b>\n"
        "Profiles are the user-defined restrictions for an application. They determine what resources applications can access or share.\n"
        "One use case for a profile is to block access to sensitive files.\n\n"
        "<u>Profiles are able to run in:</u>\n"
        "    - Complain mode: the profile does not enforce the rules defined but reports any violations to the logs\n"
        "    - Enforce mode: the profile enforces the rules defined\n\n"
        "You can change the mode of your profile(s) under the 'Profile' tab.\n\n"
        "<b>What information do logs contain?</b>\n"
        "Logs contain information for each AppArmor event, which make them useful for debugging.\n\n"
        "<u>Logs contain:</u>\n"
        "<i>Time</i>\n"
        "    - Timestamp of event logged\n"
        "<i>Type</i>\n"
        "    - DENIED: an access is denied when a profile in enforce mode violates rules\n"
        "    - STATUS: an access is allowed, but the access violated rules of a profile in complain mode\n"
        "<i>Operation</i>\n"
        "    - Profile load: load an AppArmor profile into the kernel\n"
        "    - Profile Replace: replacing current AppArmor profile\n"
        "    - Open: attempt to access file\n"
        "    - rmdir: remove directory of profile\n"
        "<i>Name</i>\n"
        "    - Full path of the executable\n\n"
        "You can access log information under the 'Logs' tab.\n\n"
        "<b>How do I monitor specific processes?</b>\n"
        "You can monitor processes with existing AppArmor profiles under the 'Processes' tab.";
}
