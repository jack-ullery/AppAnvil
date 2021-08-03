#include "random_box.h"
#include <iostream>

#include<vector>
#include<string>

RandomBox::RandomBox()
: builder{Gtk::Builder::create_from_file("./resources/random_box.glade")}
{
  builder->get_widget<Gtk::Label>("r_label", r_label);
  builder->get_widget<Gtk::Box>("r_box", r_box);

  r_label->set_text("Hell yeah dude.");

  this->add(*r_box);
  this->show_all();
}

RandomBox::~RandomBox(){
  std::cout << "Closing Random Box!" << std::endl;
  // Delete all the widgets (Haven't figured out GLib smart pointers yet!)
  delete r_box;
  delete r_label;
}