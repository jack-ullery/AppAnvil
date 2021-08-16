#include "hello_world.h"

#include <iostream>
#include<string>
#include<vector>

template <typename T_Widget>
std::unique_ptr<T_Widget> HelloWorld::get_widget(const Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder){
  T_Widget *raw_addr = nullptr;
  builder->get_widget<T_Widget>(name, raw_addr);
  return std::unique_ptr<T_Widget>(raw_addr);
}

HelloWorld::HelloWorld()
: builder{Gtk::Builder::create_from_resource("/resources/hello.glade")},
  h_label{HelloWorld::get_widget<Gtk::Label>("h_label", builder)},
  h_btn{HelloWorld::get_widget<Gtk::Button>("h_btn", builder)},
  h_box{HelloWorld::get_widget<Gtk::Box>("h_box", builder)}
{
  // Call on_button_clicked() method, when button is clicked.
  h_btn->signal_clicked().connect(sigc::mem_fun(*this, &HelloWorld::on_button_clicked));

  this->add(*h_box);
  this->show_all();
}

void HelloWorld::on_button_clicked()
{
  std::cout << messages[message_index] << std::endl;
  h_label->set_text(messages[message_index]);
  if(message_index + 1 < static_cast<int>(messages.size())){
    message_index++;
  }
}
