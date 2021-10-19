#include "dialog_world.h"

#include <gtkmm/dialog.h>
#include <gtkmm/messagedialog.h>
#include <iostream>
#include <string>
#include <vector>

DialogWorld::DialogWorld()
: builder{Gtk::Builder::create_from_file("./resources/dialog.glade")}
{
  // Get main widgets
  builder->get_widget<Gtk::Button>("h_btn",h_btn);
  builder->get_widget<Gtk::Box>("h_box",h_box);

  // Call on_button_clicked() method, when button is clicked.
  h_btn->signal_clicked().connect(sigc::mem_fun(*this, &DialogWorld::on_button_clicked));

  this->add(*h_box);
  this->show_all();
}

DialogWorld::~DialogWorld(){
  std::cout << "Closing DialogWorld!" << std::endl;
  // Please, do not do this!!!
  // This code was made without using smart pointers which is a bad practice
  // This branch is just to showcase an example of Gtk::Dialog
  delete h_box;
  delete h_btn;
}

void DialogWorld::on_button_clicked()
{
  // Associate the hw pointer with a new HelloWorld instance
  hw.reset(new HelloWorld());
  // Associate the m_diag pointer with a new Dialog
  m_diag.reset(new Gtk::Dialog("Normal Dialog Example: Hello World"));
  // Allow interaction with other parts of application when dialog is up
  m_diag->set_modal(false);

  // Add hw to the popup dialog
  m_diag->get_content_area()->pack_start(*hw, true, true); 
  // Minimum Size
  m_diag->set_size_request(DEFAULT_POPUP_WIDTH/2, DEFAULT_POPUP_HEIGHT/2); 
  // Preferred Size  
  m_diag->set_default_size(DEFAULT_POPUP_WIDTH, DEFAULT_POPUP_HEIGHT); 

  m_diag->show();  
}
