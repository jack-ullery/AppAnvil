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
    link_btn{About::get_widget<Gtk::Button>("link_btn", builder)}, a_label{About::get_widget<Gtk::Label>("a_label", builder)}

{

  a_box->set_hexpand();
  a_box->set_vexpand();
  this->add(*a_box);
  auto button_func = sigc::mem_fun(*this, &About::on_confirm_clicked);
  About::set_link_btn_signal_handler(button_func);
}


void About::set_link_btn_signal_handler(const Glib::SignalProxyProperty::SlotType &func){
        link_btn->signal_clicked().connect(func, true);
}


void About::on_confirm_clicked(){
	system("/usr/bin/firefox https://gitlab.com/apparmor/apparmor/-/wikis/Documentation");
	return;
}
