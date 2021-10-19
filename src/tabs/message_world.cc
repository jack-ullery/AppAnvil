#include "message_world.h"

#include <gtkmm/dialog.h>
#include <gtkmm/messagedialog.h>
#include <iostream>
#include <string>
#include <vector>

MessageWorld::MessageWorld()
: builder{Gtk::Builder::create_from_file("./resources/dialog.glade")}
{
  // Get main widgets
  builder->get_widget<Gtk::Button>("h_btn",h_btn);
  builder->get_widget<Gtk::Box>("h_box",h_box);

  // Call on_button_clicked() method, when button is clicked.
  h_btn->signal_clicked().connect(sigc::mem_fun(*this, &MessageWorld::on_button_clicked));

  this->add(*h_box);
  this->show_all();
}

MessageWorld::~MessageWorld(){
  std::cout << "Closing MessageWorld!" << std::endl;
  // Please, do not do this!!!
  // This code was made without using smart pointers which is a bad practice
  // This branch is just to showcase an example of Gtk::Dialog
  delete h_box;
  delete h_btn;
}

void MessageWorld::on_button_clicked()
{
  // Associate the m_diag pointer with a new Dialog
  m_diag.reset(new Gtk::MessageDialog("This is an example of a MessageDialog"));
  // Set the non-bold text
  m_diag->set_secondary_text(messages[message_index]);
  // Allow interaction with other parts of application when dialog is up
  m_diag->set_modal(false);
  // Close dialog when the OK button is pressed.
  m_diag->signal_response().connect(
    sigc::hide(sigc::mem_fun(*m_diag, &Gtk::Widget::hide)));
  m_diag->show();
  
  if(message_index + 1 < static_cast<int>(messages.size())){
    message_index++;
  }
}
