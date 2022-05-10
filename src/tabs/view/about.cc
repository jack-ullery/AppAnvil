#include "about.h"

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
    : builder{Gtk::Builder::create_from_file("./resources/about.glade")}, 
      a_box{About::get_widget<Gtk::Box>("a_box", builder)},
      a_label{About::get_widget<Gtk::Label>("a_label", builder)},
      a_search{About::get_widget<Gtk::SearchEntry>("a_search", builder)}

{
  auto search_func = sigc::mem_fun(*this, &About::on_search_changed);
  set_search_signal_handler(search_func);

  a_label->set_text(get_marked_up_text());
  a_label->set_use_markup(true);
  a_box->set_hexpand();
  a_box->set_vexpand();

  this->add(*a_box);
}

void About::set_search_signal_handler(const Glib::SignalProxyProperty::SlotType &func) {
  a_search->signal_search_changed().connect(func, true);
}

void About::on_search_changed() 
{
  std::string label = get_marked_up_text();
  std::string label_lower = get_marked_up_text_lower();
  std::string search = a_search->get_text();
  
  std::string search_lower = search;
  std::string replacement_string;

  //convert search to lower case
  transform(search.begin(), search.end(), search_lower.begin(), ::tolower);
  
  if (search.empty() || search_lower == "b" || search_lower == "i" || search_lower == "u") {
    a_label->set_text(label);
    a_label->set_use_markup(true);
    return;
  }
  
  size_t pos = label_lower.find(search_lower);

 while (pos != std::string::npos) {
    replacement_string = "<span bgcolor=\"yellow\">" + label.substr(pos, search.size()) + "</span>";
    label.replace(pos, search_lower.size(), replacement_string);
    label_lower.replace(pos, search_lower.size(), replacement_string);
    pos = label_lower.find(search_lower, pos + replacement_string.size());
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

std::string About::get_marked_up_text_lower() {
	return "<b>what is appanvil?</b>\n"
        "appanvil is a graphical user interface (gui) designed to facilitate the use of apparmor.\n\n"
        "<b>what is apparmor?</b>\n"
        "apparmor is a linux security module that protects your system from potential threats by imposing mandatory access control (mac).\n"
        "it allows the user to secure individual applications by defining and restricting what resources an application can access or share.\n\n"
        "<b>what is a profile?</b>\n"
        "profiles are the user-defined restrictions for an application. they determine what resources applications can access or share.\n"
        "one use case for a profile is to block access to sensitive files.\n\n"
        "<u>profiles are able to run in:</u>\n"
        "    - complain mode: the profile does not enforce the rules defined but reports any violations to the logs\n"
        "    - enforce mode: the profile enforces the rules defined\n\n"
        "you can change the mode of your profile(s) under the 'profile' tab.\n\n"
        "<b>what information do logs contain?</b>\n"
        "logs contain information for each apparmor event, which make them useful for debugging.\n\n"
        "<u>logs contain:</u>\n"
        "<i>time</i>\n"
        "    - timestamp of event logged\n"
        "<i>type</i>\n"
        "    - denied: an access is denied when a profile in enforce mode violates rules\n"
        "    - status: an access is allowed, but the access violated rules of a profile in complain mode\n"
        "<i>operation</i>\n"
        "    - profile load: load an apparmor profile into the kernel\n"
        "    - profile replace: replacing current apparmor profile\n"
        "    - open: attempt to access file\n"
        "    - rmdir: remove directory of profile\n"
        "<i>name</i>\n"
        "    - full path of the executable\n\n"
        "you can access log information under the 'logs' tab.\n\n"
        "<b>how do i monitor specific processes?</b>\n"
        "you can monitor processes with existing apparmor profiles under the 'processes' tab.";
}
