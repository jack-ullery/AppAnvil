#include "hello_world.h"

#include <iostream>
#include<string>
#include<vector>

HelloWorld::HelloWorld()
: builder{Gtk::Builder::create_from_file("./resources/hello.glade")}
{
  // Get main widgets
  builder->get_widget<Gtk::Label>("h_label",h_label);
  builder->get_widget<Gtk::Button>("h_btn",h_btn);
  builder->get_widget<Gtk::Box>("h_box",h_box);

  // Call on_button_clicked() method, when button is clicked.
  h_btn->signal_clicked().connect(sigc::mem_fun(*this, &HelloWorld::on_button_clicked));

  this->add(*h_box);
  this->show_all();
}

HelloWorld::~HelloWorld(){
  std::cout << "Closing HelloWorld!" << std::endl;
  delete h_box;
  delete h_btn;
  delete h_label;
}

void HelloWorld::on_button_clicked()
{
  std::cout << messages[message_index] << std::endl;
  h_label->set_text(messages[message_index]);
  if(message_index + 1 < static_cast<int>(messages.size())){
    message_index++;
  }
}
